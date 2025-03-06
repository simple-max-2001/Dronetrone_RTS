// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Comm/CommModuleComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "RTSTypes.h"
#include "Components/EntityComponent.h"
#include "Components/Comm/CommRelayComponent.h"

// Sets default values for this component's properties
UCommModuleComponent::UCommModuleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCommModuleComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const UWorld* World = GetWorld())
	{
		UpdateConnection();

		if (ConnectionUpdatePeriod > 0)
		{
			// Periodically check connection
			World->GetTimerManager().SetTimer(
				ConnectionCheckHandle, // handle to cancel timer at a later time
				this, // the owning object
				&UCommModuleComponent::UpdateConnection, // function to call on elapsed
				ConnectionUpdatePeriod, // float delay until elapsed
				true); // looping?
		}
	}
	
}

bool UCommModuleComponent::IsConnected() const
{
	return bIsConnected;
}

void UCommModuleComponent::UpdateConnection()
{
	// Check if it has connection to current relay
	if (CurrentRelay.IsValid() && CheckRelay(CurrentRelay.Get()))
	{
		bIsConnected = true;
	}
	else
	{
		// Try to find new relay
		bIsConnected = FindNewRelay();
	}
}

bool UCommModuleComponent::FindNewRelay()
{
	TArray<TSoftObjectPtr<UCommRelayComponent>> NearbyRelays = GetAvailableRelays(true);

	for (TSoftObjectPtr Relay : NearbyRelays)
	{
		if (Relay.IsValid() && CheckRelay(Relay.Get()))
		{
			CurrentRelay = TWeakObjectPtr<UCommRelayComponent>(Relay.Get());
			return true;
		}
	}

	CurrentRelay.Reset();
	return false;
}


TArray<TSoftObjectPtr<UCommModuleComponent>> UCommModuleComponent::GetAvailableComms(const bool bCheckOwnership) const
{
	TArray<AActor*> OutActors;
	TArray<TSoftObjectPtr<UCommModuleComponent>> Result;

	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetOwner()->GetActorLocation(),
		MaxSearchDistance,
		TArray<TEnumAsByte<EObjectTypeQuery>>(),
		AActor::StaticClass(),
		TArray<AActor*>(),
		OutActors
	);

	for (const AActor* Actor : OutActors)
	{
		if (const UCommModuleComponent* Comm = Actor->FindComponentByClass<UCommModuleComponent>(); Comm && Comm != this && CanCommunicateWithModule(Comm, false, bCheckOwnership))
		{
			Result.Add(Comm);
		}
	}

	return Result;
}

TArray<TSoftObjectPtr<UCommRelayComponent>> UCommModuleComponent::GetAvailableRelays(const bool bCheckOwnership) const
{
	TArray<AActor*> OutActors;
	TArray<TSoftObjectPtr<UCommRelayComponent>> Relays;

	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetOwner()->GetActorLocation(),
		MaxSearchDistance,
		TArray<TEnumAsByte<EObjectTypeQuery>>(),
		AActor::StaticClass(),
		TArray<AActor*>(),
		OutActors
	);

	for (const AActor* Actor : OutActors)
	{
		if (const UCommRelayComponent* Relay = Actor->FindComponentByClass<UCommRelayComponent>(); Relay && Relay != this && Relay->IsConnected() && CanCommunicateWithModule(Relay, true, bCheckOwnership))
		{
			Relays.Add(Relay);
		}
	}

	return Relays;
}

bool UCommModuleComponent::CheckRelay(const UCommRelayComponent* Other) const
{
	// Make reference to relay for checking connection
	const UCommRelayComponent* Relay = Other ? Other : CurrentRelay.IsValid() ? CurrentRelay.Get() : nullptr;

	// If relay is not given, or it is not connected, return false
	if (!Relay || !Relay->IsConnected()) return false;
	
	return CanCommunicateWithModule(Relay);
}

bool UCommModuleComponent::CanCommunicateWithModule(const UCommModuleComponent* Other, bool bBidirectional, bool bCheckOwnership) const
{
	// If got nullptr or address is same, return false
	if (!Other || Other == this) return false;

	// Check if owners of communication modules are valid
	const TWeakObjectPtr<AActor> OwnActor = GetOwner();
	const TWeakObjectPtr<AActor> OtherActor = Other->GetOwner();

	if (!OwnActor.IsValid() || !OtherActor.IsValid()) return false;

	if (bCheckOwnership)
	{
		UEntityComponent* OtherEntity = GET_ENTITY(OtherActor);

		if (const UEntityComponent* OwnEntity = GET_ENTITY(OwnActor);
			!OwnEntity || !OtherEntity || !OwnEntity->IsFriend(OtherEntity)) return false;
	}
	
	// If power of received signal is less than receiver sensitivity, return false
	if (GetSignalPower(Other, GetFrequency()) < GetReceiverSensitivity()) return false;
	if (bBidirectional && GetSignalPower(Other, GetFrequency(), false) < GetReceiverSensitivity()) return false;

	return true;
}

float UCommModuleComponent::GetReceiverSensitivity() const
{
	return ReceiverSensitivity;
}

float UCommModuleComponent::GetTransmitterPower() const
{
	return TransmitterPower;
}

TSoftObjectPtr<UAntennaComponent> UCommModuleComponent::GetReceiverAntenna() const
{
	return TSoftObjectPtr<UAntennaComponent>(ReceiverAntenna);
}

TSoftObjectPtr<UAntennaComponent> UCommModuleComponent::GetTransmitterAntenna() const
{
	return TSoftObjectPtr<UAntennaComponent>(TransmitterAntenna);
}

float UCommModuleComponent::GetFrequency() const
{
	return CommFrequency;
}

float UCommModuleComponent::GetSignalPower(const UCommModuleComponent* Other,
	float Frequency, const bool bSelfIsReceiver, const bool bCheckCollisions) const
{
	// If another module is null pointer, try to refer to current relay
	if (!Other)
	{
		if (CurrentRelay.IsValid())
			Other = CurrentRelay.Get();
		else
			return WEAKEST_SIGNAL;
	}

	// If talking frequency is less than zero, return weakest signal level
	if (Frequency <= 0) return WEAKEST_SIGNAL;
	
	// Define receiver and transmitter communication modules
	const UCommModuleComponent* Transmitter = bSelfIsReceiver ? Other : this;
	const UCommModuleComponent* Receiver = bSelfIsReceiver ? this : Other;

	// Try to get transmitter and receiver antennas. On fail return weakest signal level
	const TSoftObjectPtr<UAntennaComponent> TransmitterA = Transmitter->GetTransmitterAntenna();
	const TSoftObjectPtr<UAntennaComponent> ReceiverA = Receiver->GetReceiverAntenna();

	if (!TransmitterA.IsValid() || !ReceiverA.IsValid()) return WEAKEST_SIGNAL;

	// Get locations of receiver and transmitter antenna
	const FVector ReceiverLocation = ReceiverA->GetComponentLocation();
	const FVector TransmitterLocation = TransmitterA->GetComponentLocation();

	// Calculate vector from transmitter antenna to receiver antenna
	const FVector T2R = ReceiverLocation - TransmitterLocation;

	// If checking of line of sight is needed, check it, using line trace
	if (bCheckCollisions)
	{
		if (const UWorld* World = GetWorld(); World)
		{
			FCollisionParameters Params;
			
			if (FHitResult Hit; World->LineTraceSingleByChannel(Hit, TransmitterLocation, ReceiverLocation, ECC_GameTraceChannel4))
				return WEAKEST_SIGNAL;
		}
	}

	// Make sure that distance is not close to zero
	const float Distance = FMath::Max(T2R.Length(), 1.f);

	// Calculates received power P_r (dBm) using Friis Transmission Equation:
	//		P_r = P_t + G_t + G_r + 20*log_10[c/(4*pi*f*d)]
	// where
	//	* P_t - power of transmitter, dB
	//	* G_t - gain coefficient of transmitters antenna, dB
	//	* G_r - gain coefficient of receivers antenna, dB
	//	* c - speed of light, 3*10^10 cm/s
	//	* f - signal frequency, Hz
	//	* d - distance between transmitter and receiver antennas, cm
	return Transmitter->GetTransmitterPower() + TransmitterA->GetAntennaGain(T2R) + ReceiverA->GetAntennaGain(-T2R) + 20 * log10f(30.f / (4 * PI * Frequency * Distance));
}
