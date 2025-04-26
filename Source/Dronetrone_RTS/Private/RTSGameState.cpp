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
        Units.Empty();
        Buildings.Empty();
        
        // // Check current units list for invalid
        // for (const auto& Unit : Units)
        // {
        //     // Delete from units if it  isn't valid
        //     if (!Unit.IsValid() || !Unit->EntityComponent->IsAlive())
        //     {
        //         Units.Remove(Unit);
        //     }
        // }
        //
        // // Check current buildings list for invalid
        // for (const auto& Building : Buildings)
        // {
        //     // Delete from buildings if it isn't valid
        //     if (!Building.IsValid() || !Building->EntityComponent->IsAlive())
        //     {
        //         Buildings.Remove(Building);
        //     }
        // }
    
        // Get all actors of unit and building class
        TArray<AActor*> FoundUnits;
        TArray<AActor*> FoundBuildings;

        UGameplayStatics::GetAllActorsOfClass(World, ABaseUnit::StaticClass(), FoundUnits);
        UGameplayStatics::GetAllActorsOfClass(World, ABaseBuilding::StaticClass(), FoundBuildings);

        for (const auto& Actor : FoundUnits)
        {
            // Add to units list if it is valid and not in list yet
            // TSoftObjectPtr<ABaseUnit> Unit = Cast<ABaseUnit>(Actor);
            // if (Unit.IsValid())
            //     if (Unit->EntityComponent->IsAlive())
            //         Units.Add(Unit);
            
            if (TSoftObjectPtr<ABaseUnit> Unit = Cast<ABaseUnit>(Actor);
                Unit.IsValid() &&
                Unit->EntityComponent->IsAlive() &&
                !Units.Contains(Unit))
                    Units.Add(Unit);
        }

        for (const auto& Actor : FoundBuildings)
        {
            // Add to buildings list if it is valid and not in list yet
            // TSoftObjectPtr<ABaseBuilding> Building = Cast<ABaseBuilding>(Actor);
            // if (Building.IsValid())
            //     if (Building->EntityComponent->IsAlive())
            //         Buildings.Add(Building);
                
            if (TSoftObjectPtr<ABaseBuilding> Building = Cast<ABaseBuilding>(Actor);
                Building.IsValid() &&
                Building->EntityComponent->IsAlive() &&
                !Buildings.Contains(Building))
                    Buildings.Add(Building);
        }

        UE_LOG(LogTemp, Log, TEXT("Found units on map: %d"), FoundUnits.Num());
        UE_LOG(LogTemp, Log, TEXT("Found buildings on map: %d"), FoundBuildings.Num());

        UE_LOG(LogTemp, Log, TEXT("Units in list: %d"), Units.Num());
        UE_LOG(LogTemp, Log, TEXT("Buildings in list: %d"), Buildings.Num());
    }
}

TArray<TSoftObjectPtr<ABaseUnit>> ARTSGameState::GetAllUnits() const
{
    return TArray(Units);
}

TArray<TSoftObjectPtr<ABaseBuilding>> ARTSGameState::GetAllBuildings() const
{
    return TArray(Buildings);
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
