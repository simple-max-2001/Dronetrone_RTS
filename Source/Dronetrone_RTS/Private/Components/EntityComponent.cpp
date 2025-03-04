// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/EntityComponent.h"

#include "../../RTSGameMode.h"

// Sets default values for this component's properties
UEntityComponent::UEntityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
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
	// What is dead cannot die again
	if (!bIsAlive) return;

	// Change is alive to dead
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
		Owner->Destroy(); // Destroy unit
	}
}

// Called every frame
void UEntityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UEntityComponent::IsAlive() const
{
	return bIsAlive;
}

bool UEntityComponent::IsOwnedBy(int32 Owner_ID) const
{
	return GetRelation(Owner_ID) == ERelationType::Own;
}

bool UEntityComponent::IsFriend(UEntityComponent* OtherEntity, bool bOwnerOnly) const
{
	const ERelationType Relation = GetRelation(OtherEntity->GetOwnerID());
	return Relation == ERelationType::Own || Relation == ERelationType::Friend;
}

ERelationType UEntityComponent::GetRelation(int32 Owner_ID) const
{
	auto* gm = GetWorld()->GetAuthGameMode<ARTSGameMode>();

	if (gm) return gm->GetRelation(OwnerID, Owner_ID);
	
	if (OwnerID == Owner_ID) return ERelationType::Own;

	return ERelationType::Neutral;
}

float UEntityComponent::GetSelectionRadius() const
{
	return SelectionRadius;
}

int32 UEntityComponent::GetOwnerID() const
{
	return OwnerID;
}
