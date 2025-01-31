// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerState.h"
#include "RTSGameState.h"

ARTSPlayerState::ARTSPlayerState()
{

}

void ARTSPlayerState::Setup(int32 owner_id)
{
    OwnerID = owner_id;

    UpdateUnits();
}

void ARTSPlayerState::UpdateUnits()
{

    // Remove all invalid units
    for (int32 i = Units.Num()-1; i >= 0; i--)
    {
        if (!Units[i].IsValid())
        {
            Units.RemoveAt(i, 1, false);
        }
    }

    ARTSGameState* gs = GetWorld()->GetGameState<ARTSGameState>();
    if (gs)
    {
        // Add new units if they appeared
        for (TSoftObjectPtr<ABaseUnit> unit : gs->GetAllUnits())
        {
            // Skip this unit if he is not belong to players faction
            if (!unit->EntityComponent->IsOwnedBy(OwnerID)) continue;

            // If unit is not in our list, add it
            if (!Units.Contains(unit)) Units.Add(unit);
        }

        UE_LOG(LogTemp, Log, TEXT("Found units for OwnerID %d: %d"), OwnerID, Units.Num());
    }
}

TArray<TSoftObjectPtr<ABaseUnit>> ARTSPlayerState::GetAllUnits()
{
    UpdateUnits();

    return Units;
}

int32 ARTSPlayerState::GetOwnerID() const
{
    return OwnerID;
}