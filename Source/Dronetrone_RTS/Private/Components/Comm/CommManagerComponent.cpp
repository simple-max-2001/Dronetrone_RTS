// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Comm/CommManagerComponent.h"

void UCommManagerComponent::BeginPlay()
{
	Super::BeginPlay();
    
	if (const UWorld* World = GetWorld())
	{
		// Periodically check units list
		World->GetTimerManager().SetTimer(
			NetworkCheckingHandle, // handle to cancel timer at a later time
			this, // the owning object
			&UCommManagerComponent::CheckConnection, // function to call on elapsed
			NetworkCheckingInterval, // float delay until elapsed
			true); // looping?
	}
}

void UCommManagerComponent::CheckConnection() const
{
	// If there are no registered relays, do nothing
	if (!OnNetworkCheckStarted.IsBound()) return;

	OnNetworkCheckStarted.Broadcast();
}
