// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerState.h"
#include "RTSGameState.h"

ARTSPlayerState::ARTSPlayerState()
{

}

void ARTSPlayerState::Setup(EPlayerFaction player_faction)
{
    PlayerFaction = player_faction;

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
            if (!unit->SelectionComponent->IsOwnedBy(PlayerFaction)) continue;

            // If unit is not in our list, add it
            if (!Units.Contains(unit)) Units.Add(unit);
        }

        FText enum_text;
        UEnum::GetDisplayValueAsText(PlayerFaction, enum_text);

        UE_LOG(LogTemp, Log, TEXT("Found units for %s: %d"), *enum_text.ToString(), Units.Num());
    }


}

TArray<TSoftObjectPtr<ABaseUnit>> ARTSPlayerState::GetAllUnits()
{
    UpdateUnits();

    return Units;
}

EPlayerFaction ARTSPlayerState::GetPlayerFaction() const
{
    return PlayerFaction;
}