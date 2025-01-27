// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/OwnershipComponent.h"

// Sets default values for this component's properties
UOwnershipComponent::UOwnershipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOwnershipComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UOwnershipComponent::SetFaction(EPlayerFaction NewFaction)
{
	UnitFaction = NewFaction;
}

EPlayerFaction UOwnershipComponent::GetFaction() const
{
	return UnitFaction;
}

bool UOwnershipComponent::IsOwnedBy(EPlayerFaction Faction) const
{
	return UnitFaction == Faction;
}
