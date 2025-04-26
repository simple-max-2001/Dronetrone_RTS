// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Comm/CommModuleComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "RTSTypes.h"
#include "Components/EntityComponent.h"

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

	if (bIsSuper) ConnectionInfo.bIsOnline = true;
	
}

void UCommModuleComponent::UpdateConnection()
{
	if (!IsEnabled())
	{
		if (IsOnline())	ConnectionInfo = FConnectionInfo{};
		return;
	}
	
	// If module works like relay
	// if (IsRelay()) return;

	float* Power = new float(WEAKEST_SIGNAL);
	
	// Check if it has connection to current relay
	if (bIsSuper || IsAvailableRelay(nullptr, Power))
	{
		if (!IsOnline()) ConnectionInfo.bIsOnline = true;
		ConnectionInfo.Power = *Power;
	}
	else
	{
		// Try to find new relay
		FindNewRelay();
	}
}

void UCommModuleComponent::FindNewRelay()
{
	TArray<TSoftObjectPtr<UCommModuleComponent>> NearbyRelays = SearchRelays(true);

	if (!NearbyRelays.IsEmpty())
	{
		// Select relay with highest power level
		float HighestPower = WEAKEST_SIGNAL;
		float* Power = new float(WEAKEST_SIGNAL);

		TWeakObjectPtr<UCommModuleComponent> ChosenRelay;
	
		for (TSoftObjectPtr Relay : NearbyRelays)
		{
			if (IsAvailableRelay(Relay.Get(), Power) && (*Power > HighestPower))
			{
				ChosenRelay = TWeakObjectPtr<UCommModuleComponent>(Relay.Get());
				HighestPower = *Power;
			}
		}

		if (ChosenRelay.IsValid())
		{
			ConnectionInfo.bIsOnline = true;
			ConnectionInfo.Relay = ChosenRelay;
			ConnectionInfo.Power = HighestPower;
			ConnectionInfo.Frequency = GetFrequency();
		}

		return;
	}

	ConnectionInfo = FConnectionInfo{};
	return;
}

TArray<TSoftObjectPtr<UCommModuleComponent>> UCommModuleComponent::SearchRelays(const bool bCheckOwnership) const
{
	TArray<AActor*> OutActors;
	TArray<TSoftObjectPtr<UCommModuleComponent>> Relays;

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
		if (const UCommModuleComponent* Relay = Actor->FindComponentByClass<UCommModuleComponent>(); Relay && Relay != this && IsAvailableRelay(Relay))
		{
			Relays.Add(Relay);
		}
	}

	return Relays;
}

bool UCommModuleComponent::IsAvailable(const UCommModuleComponent* Other, bool bBidirectional, float* Power) const
{
	// If got nullptr or address is same, return false
	if (!Other || Other == this) return false;

	// If communicator module is disabled, return false
	if (!Other->IsEnabled()) return false;
	
	// Check if owners of communication modules are valid
	const TWeakObjectPtr<AActor> OwnActor = GetOwner();
	const TWeakObjectPtr<AActor> OtherActor = Other->GetOwner();

	if (OwnActor.IsValid() && OtherActor.IsValid())
	{
		UEntityComponent* OtherEntity = OtherActor->FindComponentByClass<UEntityComponent>();

		if (const UEntityComponent* OwnEntity = OwnActor->FindComponentByClass<UEntityComponent>();
			!OwnEntity || !OtherEntity || !OwnEntity->IsFriend(OtherEntity)) return false;
	
		// If power of received signal is less than receiver sensitivity, return false
		float receiverPower = GetSignalPower(Other, GetFrequency(), true);
		if (receiverPower < GetReceiverSensitivity() &&
			(bBidirectional && GetSignalPower(Other, GetFrequency(), false) < GetReceiverSensitivity())) return false;

		if (Power) *Power = receiverPower;
		return true;
	}

	return false;

}

bool UCommModuleComponent::IsAvailable(TWeakObjectPtr<const UCommModuleComponent> Other, bool bBidirectional, float* Power) const
{
	if (!Other.IsValid()) return false;

	return IsAvailable(Other.Get(), bBidirectional, Power);
}

bool UCommModuleComponent::IsAvailable(TSoftObjectPtr<const UCommModuleComponent> Other, bool bBidirectional, float* Power) const
{
	if (!Other.IsValid() || Other.IsPending()) return false;

	return IsAvailable(Other.Get(), bBidirectional, Power);
}

bool UCommModuleComponent::IsAvailableRelay(const UCommModuleComponent* Other, float* Power) const
{
	// Make reference to relay for checking connection
	const UCommModuleComponent* Relay = Other ? Other : ConnectionInfo.Relay.IsValid() ? ConnectionInfo.Relay.Get() : nullptr;

	// If relay is not given, or it is not connected, return false
	if (!Relay || !Relay->IsOnline() || !Relay->IsRelay()) return false;
	
	return IsAvailable(Relay, true, Power);
}

float UCommModuleComponent::GetSignalPower(const UCommModuleComponent* Other,
	float Frequency, const bool bSelfIsReceiver, const bool bCheckCollisions) const
{
	// If another module is null pointer, try to refer to current relay
	if (!Other)
	{
		if (ConnectionInfo.Relay.IsValid())
			Other = ConnectionInfo.Relay.Get();
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
	return Transmitter->GetTransmitterPower() + TransmitterAntenna->GetAntennaGain(T2R) + ReceiverAntenna->GetAntennaGain(-T2R) + 20 * log10f(30.f / (4 * PI * Frequency * Distance));
}

void UCommModuleComponent::DrawConnectionLine(const float LifeTime) const
{
	if (!ConnectionInfo.bIsOnline) return;

	if (const UWorld* World = GetWorld())
	{
		if (!ConnectionInfo.Relay.IsValid()) return;
		
		const FVector ReceiverSelf = GetReceiverAntenna()->GetComponentLocation();
		const FVector TransmitterSelf = GetTransmitterAntenna()->GetComponentLocation();
	
		const FVector ReceiverOther = ConnectionInfo.Relay->GetReceiverAntenna()->GetComponentLocation();
		const FVector TransmitterOther = ConnectionInfo.Relay->GetTransmitterAntenna()->GetComponentLocation();
		
		DrawDebugLine(World, TransmitterOther, ReceiverSelf, FColor::Green, false, LifeTime);
		DrawDebugLine(World, TransmitterSelf, ReceiverOther, FColor::Green, false, LifeTime);
	}

}
