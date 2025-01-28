// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SelectionComponent.h"

#include "../../RTSGameMode.h"

// Sets default values for this component's properties
USelectionComponent::USelectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> OwnSelectionMaterialFinder(    TEXT("/Game/Materials/Selection/M_SelectionOwn.M_SelectionOwn"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> FriendSelectionMaterialFinder( TEXT("/Game/Materials/Selection/M_SelectionFriend.M_SelectionFriend"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> NeutralSelectionMaterialFinder(TEXT("/Game/Materials/Selection/M_SelectionNeutral.M_SelectionNeutral"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> FoeSelectionMaterialFinder(    TEXT("/Game/Materials/Selection/M_SelectionFoe.M_SelectionFoe"));

    if (OwnSelectionMaterialFinder.Succeeded())
        OwnSelectionMaterial = OwnSelectionMaterialFinder.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("OwnSelectionMaterial is not setted!!!"));

    if (FriendSelectionMaterialFinder.Succeeded())
        FriendSelectionMaterial = FriendSelectionMaterialFinder.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("FriendSelectionMaterial is not setted!!!"));

    if (NeutralSelectionMaterialFinder.Succeeded())
        NeutralSelectionMaterial = NeutralSelectionMaterialFinder.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("NeutralSelectionMaterial is not setted!!!"));

    if (FoeSelectionMaterialFinder.Succeeded())
        FoeSelectionMaterial = FoeSelectionMaterialFinder.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("FoeSelectionMaterial is not setted!!!"));

}


// Called when the game starts
void USelectionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USelectionComponent::SetFaction(EPlayerFaction NewFaction)
{
	UnitFaction = NewFaction;
}

EPlayerFaction USelectionComponent::GetFaction() const
{
	return UnitFaction;
}

bool USelectionComponent::IsOwnedBy(EPlayerFaction Faction) const
{
	return UnitFaction == Faction;
}

void USelectionComponent::SetDecalComponent(UDecalComponent* decalComponent)
{
	DecalComponent = decalComponent;
}

bool USelectionComponent::IsSelected() const
{
	return bIsSelected;
}

void USelectionComponent::Select(EPlayerFaction player_faction)
{
	if (bIsSelected) return;

	if (DecalComponent)
	{
		UMaterialInterface* DecalMaterial = OwnSelectionMaterial;

		if (!IsOwnedBy(player_faction))
		{
			ARTSGameMode* game_mode = GetWorld()->GetAuthGameMode<ARTSGameMode>();

			if (game_mode)
			{
				switch (game_mode->GetRelation(UnitFaction, player_faction))
				{
					case ERelationType::FRIEND:
						DecalMaterial = FriendSelectionMaterial;
						break;

					case ERelationType::FOE:
						DecalMaterial = FoeSelectionMaterial;
						break;
					
					default:
						DecalMaterial = NeutralSelectionMaterial;
						break;
				}

			}
		}
	
		DecalComponent->SetDecalMaterial(DecalMaterial);
		DecalComponent->SetVisibility(true);
	}

	bIsSelected = true;
	OnSelectChanged.Broadcast(true);
}

void USelectionComponent::Unselect(EPlayerFaction player_faction)
{
	if (!bIsSelected) return;

	if (DecalComponent)
	{
		DecalComponent->SetVisibility(false);
	}

	bIsSelected = false;
	OnSelectChanged.Broadcast(false);
}
