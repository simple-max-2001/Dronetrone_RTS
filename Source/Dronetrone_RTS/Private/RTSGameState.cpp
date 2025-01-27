// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGameState.h"

#include "Algo/Transform.h"
#include "Kismet/GameplayStatics.h"

void ARTSGameState::ServerSetPause_Implementation(bool bPause)
{
    bIsGamePaused = bPause;
    OnRep_IsGamePaused();
}

bool ARTSGameState::ServerSetPause_Validate(bool bPause)
{
    return true;
}

void ARTSGameState::BeginPlay()
{
    UpdateUnits();

    // UpdatePlayersUnits(EPlayerFaction::PLAYER_1);
    // UpdatePlayersUnits(EPlayerFaction::PLAYER_2);
}

void ARTSGameState::AddPlayerInfo(FPlayerInfo player_info)
{
    EPlayerFaction player_id = player_info.PlayerID;

    Players.Add(player_id, player_info);
}

FPlayerInfo* ARTSGameState::GetPlayerInfo()
{
    AddPlayerInfo(FPlayerInfo());
    return &Players[EPlayerFaction::PLAYER_1];
}

void ARTSGameState::UpdateUnits()
{
    auto* world = GetWorld();

    if (world)
    {
        TArray<AActor*> FoundUnits;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseUnit::StaticClass(), FoundUnits);

        UE_LOG(LogTemp, Log, TEXT("Found units on map: %d"), FoundUnits.Num());

        if (Units.IsEmpty())
        {
            Algo::Transform(FoundUnits, Units, [](AActor* unit) -> TSoftObjectPtr<ABaseUnit>
            {
                return Cast<ABaseUnit>(unit);
            });

            TArray<EPlayerFaction> players;
            Players.GetKeys(players);

            for (EPlayerFaction player : players)
            {
                UpdatePlayersUnits(player);
            }
        }

        // TODO: Implement logic for checking unit list
    }
}

void ARTSGameState::UpdatePlayersUnits(EPlayerFaction player)
{
    FPlayerInfo* fplayer_ = &Players[player];

    fplayer_->Units.Empty(10);

    for (TSoftObjectPtr<ABaseUnit> unit : Units)
    {
        if (unit.IsValid())
        {
            if (!unit->OwnershipComponent->IsOwnedBy(player)) continue;

            fplayer_->AddUnit(unit);
        }

    }

    FText enum_text;
    UEnum::GetDisplayValueAsText(player, enum_text);

    UE_LOG(LogTemp, Log, TEXT("Found units for %s: %d"), *enum_text.ToString(), Players[player].Units.Num());
}

void ARTSGameState::OnRep_IsGamePaused()
{
    if (bIsGamePaused)
    {
        UGameplayStatics::SetGamePaused(GetWorld(), true);
    }
    else
    {
        UGameplayStatics::SetGamePaused(GetWorld(), false);
    }
}
