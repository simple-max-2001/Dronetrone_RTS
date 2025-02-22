// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Comm/CommModuleComponent.h"
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

	// Check if have connection to current relay
	if (CurrentRelay.IsValid() && CurrentRelay->IsConnected())
	{
		bConnected = true;
	}
	else
	{
		// TODO: Add logic for search of new relay to connect
	}

	bIsConnected = bConnected;
}
