// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once

#include "Components/EntityComponent.h"

// Sets default values for this component's properties
UEntityComponent::UEntityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	//OnDieEntity.Broadcast();
	//OnDestroyEntity.Broadcast();
}


// Called when the game starts
void UEntityComponent::BeginPlay()
{
	Super::BeginPlay();

	// Call blueprint event OnSpawnEntity
	if (OnSpawnEntity.IsBound()) OnSpawnEntity.Broadcast();
	
}

void UEntityComponent::KillEntity()
{
	// Change status
	bIsAlive = false;

	if (OnDieEntity.IsBound()) OnDieEntity.Broadcast();
}

void UEntityComponent::DestroyEntity()
{
	// Execute OnDestroyEntity
	if (OnDestroyEntity.IsBound()) OnDestroyEntity.Broadcast();
	
	// Destoy actor owner
	AActor* Owner = GetOwner();
	if (IsValid(Owner))
	{
		Owner->Destroy(); // Актор буде знищено через 2 секунди
	}
}

// Called every frame
void UEntityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UEntityComponent::IsAlive()
{
	return bIsAlive;
}

