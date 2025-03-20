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
        UpdateEntities();

        // Periodically check units list
        World->GetTimerManager().SetTimer(
            EntitiesCheckingHandle,
            this,
            &ARTSGameState::UpdateEntities,
            UpdatePeriod,
            true);
    }
}

void ARTSGameState::UpdateEntities()
{
    if (const UWorld* World = GetWorld())
    {
        // Check current units list for invalid
        for (const auto& Unit : Units)
        {
            // Delete from units if it  isn't valid
            if (!Unit.IsValid())
            {
                Units.Remove(Unit);
            }
        }
        
        // Check current buildings list for invalid
        for (int32 i = Buildings.Num()-1; !Buildings.IsEmpty() && i >= 0; i--)
        {
            // Delete from buildings if it isn't valid
            if (!Buildings[i].IsValid())
            {
                Buildings.RemoveAt(i, 1, false);
            }
        }
    
        // Get all actors of unit and building class
        TArray<AActor*> FoundUnits;
        TArray<AActor*> FoundBuildings;

        UGameplayStatics::GetAllActorsOfClass(World, ABaseUnit::StaticClass(), FoundUnits);
        UGameplayStatics::GetAllActorsOfClass(World, ABaseBuilding::StaticClass(), FoundBuildings);

        for (int32 i = 0; i < FoundUnits.Num(); i++)
        {
            // Add to units list if it is valid and not in list yet
            if (TSoftObjectPtr<ABaseUnit> Unit = Cast<ABaseUnit>(FoundUnits[i]); Unit.IsValid() && Units.Contains(Unit))
            {
                Units.Add(Unit);
            }
        }

        for (int32 i = 0; i < FoundBuildings.Num(); i++)
        {
            // Add to buildings list if it is valid and not in list yet
            if (TSoftObjectPtr<ABaseBuilding> Building = Cast<ABaseUnit>(FoundBuildings[i]); Building.IsValid() && Buildings.Contains(Building))
            {
                Buildings.Add(Building);
            }
        }

        UE_LOG(LogTemp, Log, TEXT("Found units on map: %d"), FoundUnits.Num());
        UE_LOG(LogTemp, Log, TEXT("Found buildings on map: %d"), FoundBuildings.Num());
    }
}

TArray<TSoftObjectPtr<ABaseUnit>> ARTSGameState::GetAllUnits() const
{
    return Units;
}

TArray<TSoftObjectPtr<ABaseBuilding>> ARTSGameState::GetAllBuildings() const
{
    return Buildings;
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
