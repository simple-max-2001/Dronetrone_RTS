// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectionManager.h"

// Sets default values
ASelectionManager::ASelectionManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASelectionManager::SetOwnerID(int32 Owner_ID)
{
	OwnerID = Owner_ID;
}

void ASelectionManager::SetKeepSelection(bool bKeep_Selection)
{
    bKeepSelection = bKeep_Selection;
}

void ASelectionManager::Select(TWeakObjectPtr<AActor> Entity)
{
    if (bKeepSelection)
    {
        ABaseUnit* Unit = Cast<ABaseUnit>(Entity);

        if (!Unit) return;

        if (SelectedUnits.Contains(Unit)) RemoveFromSelection(Unit);
        else AddToSelection(Unit);
    }
    else
    {
        RemoveAllFromSelection();
        AddToSelection(Entity.Get());
    }

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::Select(TArray<AActor*> Entities)
{
    if (!bKeepSelection) RemoveAllFromSelection();

    for (AActor* Entity : Entities)
    {
        // Apply multiple selection only if entity is unit
        if (!Cast<ABaseUnit>(Entity)) continue; 

        AddToSelection(Entity);
    }

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::Select(TArray<TWeakObjectPtr<AActor>> Entities)
{
    if (!bKeepSelection) RemoveAllFromSelection();

    for (TWeakObjectPtr Entity : Entities)
    {
        // Apply multiple selection only if entity is unit
        if (!Cast<ABaseUnit>(Entity)) continue; 

        AddToSelection(Entity.Get());
    }

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::Select(TWeakObjectPtr<ABaseUnit> Unit)
{
    if (bKeepSelection)
    {
        if (SelectedUnits.Contains(Unit)) RemoveFromSelection(Unit.Get());
        else AddToSelection(Unit.Get());
    }
    else
    {
        RemoveAllFromSelection();
        AddToSelection(Unit.Get());
    }

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::Select(TArray<ABaseUnit*> Units)
{
    if (!bKeepSelection) RemoveAllFromSelection();

    for (AActor* Unit : Units)
    {
        AddToSelection(Unit);
    }

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::Select(TArray<TWeakObjectPtr<ABaseUnit>> Units)
{
    if (!bKeepSelection) RemoveAllFromSelection();

    for (TWeakObjectPtr<AActor> Unit : Units)
    {
        AddToSelection(Unit.Get());
    }

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::DeselectAll()
{
    if (SelectedUnits.IsEmpty()) return;

    RemoveAllFromSelection();

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::CheckSelection()
{
    bool bGotChanges = false;
    for (auto It = SelectedUnits.CreateIterator(); It; ++It)
    {
        TWeakObjectPtr<ABaseUnit> Unit = *It;

        if (!Unit.IsValid() ||                          // If unit is destroyed
            !Unit->EntityComponent->IsAlive() ||        // If unit is dead
            !Unit->EntityComponent->IsOwnedBy(OwnerID)) // If is not owned by player
        {
            It.RemoveCurrent(); // Remove from list
            bGotChanges = true;
        }
    }

    if (bGotChanges) OnSelectionChanged.Broadcast();
}

TArray<TWeakObjectPtr<ABaseUnit>> ASelectionManager::GetSelectedUnits() const
{
   return TArray(SelectedUnits);
}

void ASelectionManager::AddToSelection(AActor* Entity)
{
    if (ABaseUnit* Unit = Cast<ABaseUnit>(Entity); Unit) AddToSelection(Unit);
}

void ASelectionManager::AddToSelection(ABaseUnit* Unit)
{
    if (Unit->EntityComponent->IsAlive() &&
        Unit->EntityComponent->IsOwnedBy(OwnerID) &&
        !SelectedUnits.Contains(Unit))
    {
        Unit->EntityComponent->OnDieEntity.AddDynamic(this, &ASelectionManager::CheckSelection);
        Unit->EntityComponent->OnDestroyEntity.AddDynamic(this, &ASelectionManager::CheckSelection);
        SelectedUnits.Add(Unit);
    }
}

void ASelectionManager::RemoveFromSelection(ABaseUnit* Unit)
{
    if (!SelectedUnits.Contains(Unit)) return;

    // Unsubscribe from die and destroy delegates
    // if (unit_w.IsValid())
    {
        Unit->EntityComponent->OnDieEntity.RemoveDynamic(this, &ASelectionManager::CheckSelection);
        Unit->EntityComponent->OnDestroyEntity.RemoveDynamic(this, &ASelectionManager::CheckSelection);
    }

    SelectedUnits.Remove(Unit);
}

void ASelectionManager::RemoveAllFromSelectionBut(ABaseUnit* Unit)
{
    for (auto It = SelectedUnits.CreateIterator(); It; ++It)
    {
        TWeakObjectPtr<ABaseUnit> UnitIt = *It;
        
        if ((!UnitIt.IsValid() ||                          // If unit is destroyed
             !UnitIt->EntityComponent->IsAlive() ||        // If unit is dead
             !UnitIt->EntityComponent->IsOwnedBy(OwnerID)) // If is not owned by player
            && Unit != UnitIt.Get())
        {
            It.RemoveCurrent(); // Remove from list
        }
    }
}

void ASelectionManager::RemoveAllFromSelection()
{
    SelectedUnits.Empty();
}
