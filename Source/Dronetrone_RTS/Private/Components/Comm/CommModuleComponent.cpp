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

	UpdateConnection();
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

bool UCommModuleComponent::CheckRelay(UCommRelayComponent* Other) const
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
		UEntityComponent* OwnEntity = GET_ENTITY(OwnActor);
		UEntityComponent* OtherEntity = GET_ENTITY(OtherActor);

		if (!OwnEntity || !OtherEntity || !OwnEntity->IsFriend(OtherEntity)) return false;
	}

	const FVector OwnLocation = OwnActor->GetActorLocation();
	const FVector OtherLocation = OtherActor->GetActorLocation();

	// Vector from own module to another module
	const FVector Distance = OtherLocation - OwnLocation;
	
	// If power of received signal is less than receiver sensitivity, return false
	if (GetSignalPower(this, Other, Distance, GetFrequency()) < GetReceiverSensitivity()) return false;
	if (bBidirectional && GetSignalPower(Other, this, -Distance, GetFrequency()) < GetReceiverSensitivity()) return false;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnActor.Get());
	Params.AddIgnoredActor(OtherActor.Get());

	// Check if another actor is in line of sight
	return !GetWorld()->LineTraceSingleByChannel(Hit, OwnLocation, OtherLocation, ECC_GameTraceChannel4, Params);
}

float UCommModuleComponent::GetReceiverSensitivity() const
{
	return ReceiverSensitivity;
}

float UCommModuleComponent::GetTransmitterPower() const
{
	return TransmitterPower;
}

float UCommModuleComponent::GetReceiverGain(FVector Direction) const
{
	return ReceiverGain;
}

float UCommModuleComponent::GetTransmitterGain(FVector Direction) const
{
	return TransmitterGain;
}

float UCommModuleComponent::GetFrequency() const
{
	return Frequency;
}

float UCommModuleComponent::GetSignalPower(const UCommModuleComponent* Transmitter, const UCommModuleComponent* Receiver,
	const FVector Distance, const float Frequency)
{
	// Calculates logarithmic received power P_r (dB) using Friis Transmission Equation:
	//		P_r = P_t + G_t + G_r + 20*log_10[c/(4*pi*f*d)]
	// where
	//	* P_t - power of transmitter, dB
	//	* G_t - gain coefficient of transmitters antenna, dB
	//	* G_r - gain coefficient of receivers antenna, dB
	//	* c - speed of light, 3*10^10 cm/s
	//	* f - signal frequency, Hz
	//	* d - distance between transmitter and receiver antennas, cm
	
	return Transmitter->GetTransmitterPower() + Transmitter->GetTransmitterGain(Distance) + Receiver->GetReceiverGain(-Distance) + 20 * log10f(30.f / (4 * PI * Frequency * Distance.Length()));
}
