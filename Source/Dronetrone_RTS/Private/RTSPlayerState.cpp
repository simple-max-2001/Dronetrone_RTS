// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerState.h"
#include "RTSGameState.h"

ARTSPlayerState::ARTSPlayerState()
{

}

void ARTSPlayerState::Setup(int32 Owner_ID)
{
    OwnerID = Owner_ID;

    UpdateEntities();
}

void ARTSPlayerState::UpdateEntities()
{
    // Remove all invalid units
    for (const auto& Unit : Units)
    {
        if (!Unit.IsValid()) Units.Remove(Unit);
    }
    
    // Remove all invalid buildings
    for (const auto& Building : Buildings)
    {
        if (!Building.IsValid()) Buildings.Remove(Building);
    }

    if (const ARTSGameState* GS = GetWorld()->GetGameState<ARTSGameState>())
    {
        // Add new units if they appeared
        for (TSoftObjectPtr Unit : GS->GetAllUnits())
        {
            // Skip this unit if it is not belong to players faction
            if (!Unit->EntityComponent->IsOwnedBy(OwnerID)) continue;

            // If unit is not in our list, add it
            if (!Units.Contains(Unit)) Units.Add(Unit);
        }

        // Add new buildings if they appeared
        for (TSoftObjectPtr Building : GS->GetAllBuildings())
        {
            // Skip this building if it is not belong to players faction
            if (!Building->EntityComponent->IsOwnedBy(OwnerID)) continue;

            // If building is not in our list, add it
            if (!Buildings.Contains(Building)) Buildings.Add(Building);
        }

        UE_LOG(LogTemp, Log, TEXT("Found units for OwnerID %d: %d"), OwnerID, Units.Num());
        UE_LOG(LogTemp, Log, TEXT("Found buildings for OwnerID %d: %d"), OwnerID, Buildings.Num());
    }
}

TArray<TSoftObjectPtr<ABaseUnit>> ARTSPlayerState::GetAllUnits() const
{
    return Units;
}

int32 ARTSPlayerState::GetOwnerID() const
{
    return OwnerID;
}