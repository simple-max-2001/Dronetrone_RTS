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

    // Periodically check units list
    GetWorld()->GetTimerManager().SetTimer(
        UnitsCheckingHandle, // handle to cancel timer at a later time
        this, // the owning object
        &ARTSGameState::UpdateUnits, // function to call on elapsed
        1.f, // float delay until elapsed
        true); // looping?
}

void ARTSGameState::UpdateUnits()
{
    auto* world = GetWorld();

    if (world)
    {
        TArray<AActor*> FoundUnits;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseUnit::StaticClass(), FoundUnits);

        UE_LOG(LogTemp, Log, TEXT("Found units on map: %d"), FoundUnits.Num());

        Units.Empty(10);

        Algo::Transform(FoundUnits, Units, [](AActor* unit) -> TSoftObjectPtr<ABaseUnit>
        {
            return Cast<ABaseUnit>(unit);
        });
    }
}

TArray<TSoftObjectPtr<ABaseUnit>> ARTSGameState::GetAllUnits()
{
    UpdateUnits();

    return Units;
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
