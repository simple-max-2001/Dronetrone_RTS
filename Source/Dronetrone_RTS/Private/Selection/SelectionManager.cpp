// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectionManager.h"

// Sets default values
ASelectionManager::ASelectionManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASelectionManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASelectionManager::SetOwnerID(int32 owner_id)
{
	OwnerID = owner_id;
}

void ASelectionManager::SetKeepSelection(bool keep_selection)
{
    UE_LOG(LogTemp, Warning, TEXT("KeepSelection %d"), keep_selection);
    bKeepSelection = keep_selection;
}

void ASelectionManager::SelectUnit(TSoftObjectPtr<ABaseUnit> unit, bool deselect)
{
    if (bKeepSelection)
    {
        if (SelectedUnits.Contains(unit))
        {
            if (deselect) DeselectUnit(unit);
            return;
        }
    }
    else
    {
        if (deselect) DeselectAll();
    }

    if (!unit.IsValid() || !unit->EntityComponent->IsOwnedBy(OwnerID)) return;

    // Make selection indicator
    ASelectionIndicator* indicator = GetWorld()->SpawnActor<ASelectionIndicator>();
    if (indicator)
    {
        if (indicator->AttachToEntity(unit, OwnerID))
		{
			UE_LOG(LogTemp, Log, TEXT("SelectUnit"));
		    unit->EntityComponent->OnDieEntity.AddDynamic(this, &ASelectionManager::CheckSelection);
		    unit->EntityComponent->OnDestroyEntity.AddDynamic(this, &ASelectionManager::CheckSelection);
        	SelectedUnits.Add(unit, indicator);
		}
    }
}

void ASelectionManager::SelectUnits(TArray<ABaseUnit*> units)
{
    if (!bKeepSelection) DeselectAll();

    for (ABaseUnit* unit : units)
    {
        SelectUnit(unit, false);
    }
}

void ASelectionManager::SelectUnits(TArray<TSoftObjectPtr<ABaseUnit>> units)
{
    if (!bKeepSelection) DeselectAll();

    for (TSoftObjectPtr<ABaseUnit> unit : units)
    {
        SelectUnit(unit, false);
    }
}

void ASelectionManager::DeselectUnit(TSoftObjectPtr<ABaseUnit> unit)
{
    if (!SelectedUnits.Contains(unit)) return;

	UE_LOG(LogTemp, Log, TEXT("DeselectUnit"));
	
	// Unsubscribe from die and destroy delegates
	if (unit.IsValid())
	{
		unit->EntityComponent->OnDieEntity.RemoveDynamic(this, &ASelectionManager::CheckSelection);
		unit->EntityComponent->OnDestroyEntity.RemoveDynamic(this, &ASelectionManager::CheckSelection);
	}
	TSoftObjectPtr<ASelectionIndicator> indicator = SelectedUnits[unit];

	if (indicator.IsValid()) indicator->Destroy();

	SelectedUnits.Remove(unit);
}

void ASelectionManager::DeselectAll()
{
    if (SelectedUnits.IsEmpty()) return;

    for (auto& Pair : SelectedUnits)
    {
		if (Pair.Key.IsValid())
		{
			Pair.Key->EntityComponent->OnDieEntity.RemoveDynamic(this, &ASelectionManager::CheckSelection);
			Pair.Key->EntityComponent->OnDestroyEntity.RemoveDynamic(this, &ASelectionManager::CheckSelection);
		}
        if (Pair.Value.IsValid())
            Pair.Value->Destroy();
    }

    SelectedUnits.Empty();
}

void ASelectionManager::CheckSelection()
{
    for (auto It = SelectedUnits.CreateIterator(); It; ++It)
    {
        if (!It.Key().IsValid() || !It.Key()->EntityComponent->IsAlive()) // If unit is destroyed or dead
        {
            if (It.Value().IsValid()) // Destroy indicator
            {
                It.Value()->Destroy();
            }
            It.RemoveCurrent(); // Remove from list
        }
    }
}

TArray<TSoftObjectPtr<ABaseUnit>> ASelectionManager::GetSelectedUnits()
{
	CheckSelection();

    TArray<TSoftObjectPtr<ABaseUnit>> Units;
    SelectedUnits.GenerateKeyArray(Units);
    return Units;
}
