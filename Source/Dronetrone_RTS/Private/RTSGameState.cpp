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

    UpdatePlayersUnits(EOwnership::PLAYER_1);



    //AddPlayerInfo(FPlayerInfo());
    //AddPlayerInfo(FPlayerInfo(EOwnership::PLAYER_2, false));
    //AddPlayerInfo(FPlayerInfo(EOwnership::PLAYER_3, false));
}

void ARTSGameState::AddPlayerInfo(FPlayerInfo player_info)
{
    EOwnership player_id = player_info.PlayerID;

    Players.Add(player_id, player_info);
    UpdatePlayersUnits(player_id);

    FText enum_text;
    UEnum::GetDisplayValueAsText(player_id, enum_text);

    UE_LOG(LogTemp, Log, TEXT("Found units for %s: %d"), *enum_text.ToString(), Players[player_id].Units.Num());
}

FPlayerInfo* ARTSGameState::GetPlayerInfo()
{
    AddPlayerInfo(FPlayerInfo());
    return &Players[EOwnership::PLAYER_1];
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
        }

        // TODO: Implement logic for checking unit list
    }
}

void ARTSGameState::UpdatePlayersUnits(EOwnership player)
{
    FPlayerInfo* fplayer_ = &Players[player];

    fplayer_->Units.Empty(10);

    for (TSoftObjectPtr<ABaseUnit> unit : Units)
    {
        if (unit->EntityComponent->GetOwnerID() != player) continue;

        fplayer_->AddUnit(unit);
    }
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
