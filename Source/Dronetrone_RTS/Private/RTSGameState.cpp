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
	Super::BeginPlay();
    
    if (const UWorld* World = GetWorld())
    {
        UpdateUnits();

        // Periodically check units list
        World->GetTimerManager().SetTimer(
            UnitsCheckingHandle, // handle to cancel timer at a later time
            this, // the owning object
            &ARTSGameState::UpdateUnits, // function to call on elapsed
            1.f, // float delay until elapsed
            true); // looping?
    }
}

void ARTSGameState::UpdateUnits()
{
    if (const UWorld* World = GetWorld())
    {
        // Check current units list for invalid
        for (int32 i = Units.Num()-1; !Units.IsEmpty() && i >= 0; i--)
        {
            // Delete from units it not valid
            if (!Units[i].IsValid())
            {
                Units.RemoveAt(i, 1, false);
            }
        }
    
        // Get all actor of unit class
        TArray<AActor*> FoundUnits;
        UGameplayStatics::GetAllActorsOfClass(World, ABaseUnit::StaticClass(), FoundUnits);

        for (int32 i = 0; i < FoundUnits.Num(); i++)
        {
            // Add to units list if it is valid and not in list yet
            if (TSoftObjectPtr<ABaseUnit> Unit = Cast<ABaseUnit>(FoundUnits[i]); Unit.IsValid() && Units.Contains(Unit))
            {
                Units.Add(Unit);
            }
        }

        Units.Empty(10);

        Algo::Transform(FoundUnits, Units, [](AActor* Entity) -> TSoftObjectPtr<ABaseUnit>
        {
            return Cast<ABaseUnit>(Entity);
        });

        UE_LOG(LogTemp, Log, TEXT("Found units on map: %d"), FoundUnits.Num());
    }
}

TArray<TSoftObjectPtr<ABaseUnit>> ARTSGameState::GetAllUnits()
{
    UpdateUnits();

    return Units;
}

void ARTSGameState::OnRep_IsGamePaused()
{
    if (const UWorld* World = GetWorld())
    {
        if (bIsGamePaused)
        {
            UGameplayStatics::SetGamePaused(World, true);
        }
        else
        {
            UGameplayStatics::SetGamePaused(World, false);
        }
    }
}
