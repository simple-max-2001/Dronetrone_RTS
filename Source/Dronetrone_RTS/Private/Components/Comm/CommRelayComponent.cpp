// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Comm/CommRelayComponent.h"
#include "RTSTypes.h"
#include "Components/EntityComponent.h"

void UCommRelayComponent::ResetChecked()
{
	bIsChecked = false;
}

void UCommRelayComponent::MarkAsChecked()
{
	bIsChecked = CurrentRelay.IsValid();
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
	
	// Super::UpdateConnection();
}
