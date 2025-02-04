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

void ASelectionManager::SelectUnit(TSoftObjectPtr<ABaseUnit> unit, bool deselect, bool broadcast)
{
    if (bKeepSelection)
    {
        if (SelectedUnits.Contains(unit))
        {
            if (deselect) DeselectUnit(unit, broadcast);
            return;
        }
    }
    else
    {
        if (deselect) DeselectAll(false);
    }

    if (unit.IsValid() && unit->EntityComponent->IsOwnedBy(OwnerID))
    {
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

    if (broadcast) OnSelectionChanged.Broadcast();
}

void ASelectionManager::SelectUnits(TArray<ABaseUnit*> units, bool broadcast)
{
    if (!bKeepSelection) DeselectAll(false);

    for (ABaseUnit* unit : units)
    {
        SelectUnit(unit, false, false);
    }

    if (broadcast) OnSelectionChanged.Broadcast();
}

void ASelectionManager::SelectUnits(TArray<TSoftObjectPtr<ABaseUnit>> units, bool broadcast)
{
    if (!bKeepSelection) DeselectAll(false);

    for (TSoftObjectPtr<ABaseUnit> unit : units)
    {
        SelectUnit(unit, false, false);
    }

    if (broadcast) OnSelectionChanged.Broadcast();
}

void ASelectionManager::DeselectUnit(TSoftObjectPtr<ABaseUnit> unit, bool broadcast)
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
    if (broadcast) OnSelectionChanged.Broadcast();
}

void ASelectionManager::DeselectAll(bool broadcast)
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
    if (broadcast) OnSelectionChanged.Broadcast();
}

void ASelectionManager::CheckSelection()
{
    bool got_changes = false;
    for (auto It = SelectedUnits.CreateIterator(); It; ++It)
    {
        if (!It.Key().IsValid() ||                          // If unit is destroyed
            !It.Key()->EntityComponent->IsAlive() ||        // If unit is dead
            !It.Key()->EntityComponent->IsOwnedBy(OwnerID)) // If is not owned by player
        {
            if (It.Value().IsValid()) // Destroy indicator
            {
                It.Value()->Destroy();
            }
            It.RemoveCurrent(); // Remove from list
            got_changes = true;
        }
    }

    if (got_changes) OnSelectionChanged.Broadcast();
}

TArray<TSoftObjectPtr<ABaseUnit>> ASelectionManager::GetSelectedUnits()
{
	CheckSelection();

    TArray<TSoftObjectPtr<ABaseUnit>> Units;
    SelectedUnits.GenerateKeyArray(Units);
    return Units;
}
