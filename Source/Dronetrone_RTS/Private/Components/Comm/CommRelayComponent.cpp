// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Comm/CommRelayComponent.h"
#include "RTSTypes.h"
#include "Components/EntityComponent.h"

UCommRelayComponent::UCommRelayComponent() : Super()
{
	ConnectionUpdatePeriod = 0.f;
}

void UCommRelayComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCommRelayComponent::ResetChecked()
{
	bIsChecked = false;
}

void UCommRelayComponent::MarkAsChecked()
{
	bIsChecked = CurrentRelay.IsValid();
}

bool UCommRelayComponent::TryToConnectWith(const UCommRelayComponent* Relay, bool bOwnOnly)
{
	if (CheckRelay(Relay))
	{
		CurrentRelay = TWeakObjectPtr<const UCommRelayComponent>(Relay);
		return true;
	}
	
	return false;
}

void UCommRelayComponent::UpdateConnection()
{
	if (bIsSuper)
	{
		bIsConnected = true;
		return;
	}

	if (CurrentRelay.IsValid())
	{
		const AActor* RelayOwner = CurrentRelay->GetOwner();
	
		bIsConnected = bIsChecked && CurrentRelay.IsValid() && RelayOwner && GET_ENTITY(RelayOwner)->IsAlive();
	}
}

bool UCommRelayComponent::GetIsSuper() const
{
	return bIsSuper;
}
