// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Comm/CommModuleComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Components/Comm/CommRelayComponent.h"

// Sets default values for this component's properties
UCommModuleComponent::UCommModuleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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
	bool bConnected = false;

	// Check if it has connection to current relay
	if (CurrentRelay.IsValid() && CheckRelay(CurrentRelay.Get()))
	{
		bConnected = true;
	}
	else
	{
		// Try to find new relay for connection with new one relay
		bConnected = FindNewRelay();
	}

	bIsConnected = bConnected;
}

bool UCommModuleComponent::FindNewRelay()
{
	return false;
}

bool UCommModuleComponent::CheckRelay(UCommRelayComponent* Other) const
{
	// Make reference to relay for checking connection
	UCommRelayComponent* Relay = Other ? Other : CurrentRelay.IsValid() ? CurrentRelay.Get() : nullptr;

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

	const FVector OwnLocation = OwnActor->GetActorLocation();
	const FVector OtherLocation = OtherActor->GetActorLocation();

	const FVector Distance = OtherLocation - OwnLocation;
	
	// If power of received signal is less than receiver sensitivity, return false
	if (GetSignalPower(this, Other, Distance, GetFrequency()) < GetReceiverSensitivity()) return false;

	FHitResult Hit;

	// Check if another actor is in line of sight
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnActor.Get());
	Params.AddIgnoredActor(OtherActor.Get());

	return !GetWorld()->LineTraceSingleByChannel(Hit, OwnLocation, OtherLocation, ECC_Visibility, Params);
}

TArray<TSoftObjectPtr<UCommModuleComponent>> UCommModuleComponent::GetCommNeighbours(bool bCheckOwnership, bool bRelaysOnly) const
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

	for (AActor* Actor : OutActors)
	{
		UCommModuleComponent* Comm;

		if (bRelaysOnly) Comm = Actor->FindComponentByClass<UCommRelayComponent>();
		else Comm = Actor->FindComponentByClass<UCommModuleComponent>();
		
		if (Comm && Comm != this && CanCommunicateWithModule(Comm, true, bCheckOwnership))
		{
			Result.Add(Comm);
		}
	}

	return Result;
}

double UCommModuleComponent::GetReceiverSensitivity() const
{
	return ReceiverSensitivity;
}

double UCommModuleComponent::GetTransmitterPower() const
{
	return TransmitterPower;
}

double UCommModuleComponent::GetReceiverGain(FVector Direction) const
{
	return ReceiverGain;
}

double UCommModuleComponent::GetTransmitterGain(FVector Direction) const
{
	return TransmitterGain;
}

double UCommModuleComponent::GetFrequency() const
{
	return Frequency;
}

double UCommModuleComponent::GetSignalPower(const UCommModuleComponent* Transmitter, const UCommModuleComponent* Receiver,
	const FVector Distance, const float Frequency)
{
	// Calculates received power P_r using Friis Transmission Equation:
	//		P_r = P_t * G_t * G_r * (c/(4*pi*f*d))^2
	// where
	//	* P_t - power of transmitter
	//	* G_t - gain coefficient of transmitters antenna
	//	* G_r - gain coefficient of receivers antenna
	//	* c - speed of light 3*10^8 m/s (3*10^10 cm/s)
	//	* f - signal frequency, Hz
	//	* d - distance between transmitter and receiver antennas
	
	const double Coefficient = 3e10f / (4 * PI * Frequency * Distance.Length());
	return Transmitter->GetTransmitterPower() * Transmitter->GetTransmitterGain(Distance) * Receiver->GetReceiverGain(-Distance) * Coefficient * Coefficient;
}
