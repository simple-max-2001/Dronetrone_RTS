// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerState.h"
#include "RTSGameState.h"

ARTSPlayerState::ARTSPlayerState()
{

}

void ARTSPlayerState::Setup(int32 Owner_ID)
{
    OwnerID = Owner_ID;
    
    if (const UWorld* World = GetWorld())
    {
        UpdateEntities();

        // Periodically check units list
        World->GetTimerManager().SetTimer(
            UpdateEntitiesHandle,
            this,
            &ARTSPlayerState::UpdateEntities,
            UpdatePeriod,
            true);
    }
}

void ARTSPlayerState::UpdateEntities()
{
    Units.Empty();
    Buildings.Empty();

    if (const ARTSGameState* GS = GetWorld()->GetGameState<ARTSGameState>())
    {
        // Add new units if they appeared
        for (TSoftObjectPtr Unit : GS->GetAllUnits())
        {
            // Skip this unit if it is not belong to players faction
            if (!Unit.IsValid() && !Unit->EntityComponent->IsOwnedBy(OwnerID)) continue;

            // If unit is not in our list, add it
            Units.Add(Unit);
        }

        // Add new buildings if they appeared
        for (TSoftObjectPtr Building : GS->GetAllBuildings())
        {
            // Skip this building if it is not belong to players faction
            if (!Building.IsValid() && !Building->EntityComponent->IsOwnedBy(OwnerID)) continue;

            // If building is not in our list, add it
            Buildings.Add(Building);
        }

        // UE_LOG(LogTemp, Log, TEXT("Found units for OwnerID %d: %d"), OwnerID, Units.Num());
        // UE_LOG(LogTemp, Log, TEXT("Found buildings for OwnerID %d: %d"), OwnerID, Buildings.Num());
    }
}