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

void ASelectionManager::Select(TWeakObjectPtr<AActor> entity)
{
    if (bKeepSelection)
    {
        ABaseUnit* unit = Cast<ABaseUnit>(entity);

        if (!unit) return;

        if (SelectedUnits.Contains(unit)) RemoveFromSelection(unit);
        else AddToSelection(unit);
    }
    else
    {
        RemoveAllFromSelection();
        AddToSelection(entity.Get());
    }

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::Select(TArray<AActor*> entities)
{
    if (!bKeepSelection) RemoveAllFromSelection();

    for (AActor* entity : entities)
    {
        // Apply multiple selection only if entity is unit
        if (!Cast<ABaseUnit>(entity)) continue; 

        AddToSelection(entity);
    }

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::Select(TArray<TWeakObjectPtr<AActor>> entities)
{
    if (!bKeepSelection) RemoveAllFromSelection();

    for (TWeakObjectPtr<AActor> entity : entities)
    {
        // Apply multiple selection only if entity is unit
        if (!Cast<ABaseUnit>(entity)) continue; 

        AddToSelection(entity.Get());
    }

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::Select(TWeakObjectPtr<ABaseUnit> unit)
{
    if (bKeepSelection)
    {
        if (SelectedUnits.Contains(unit)) RemoveFromSelection(unit.Get());
        else AddToSelection(unit.Get());
    }
    else
    {
        RemoveAllFromSelection();
        AddToSelection(unit.Get());
    }

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::Select(TArray<ABaseUnit*> units)
{
    if (!bKeepSelection) RemoveAllFromSelection();

    for (AActor* unit : units)
    {
        AddToSelection(unit);
    }

    OnSelectionChanged.Broadcast();
}

void ASelectionManager::Select(TArray<TWeakObjectPtr<ABaseUnit>> units)
{
    if (!bKeepSelection) RemoveAllFromSelection();

    for (TWeakObjectPtr<AActor> unit : units)
    {
        AddToSelection(unit.Get());
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
    bool got_changes = false;
    for (auto It = SelectedUnits.CreateIterator(); It; ++It)
    {
        TWeakObjectPtr<ABaseUnit> unit = *It;

        if (!unit.IsValid() ||                          // If unit is destroyed
            !unit->EntityComponent->IsAlive() ||        // If unit is dead
            !unit->EntityComponent->IsOwnedBy(OwnerID)) // If is not owned by player
        {
            It.RemoveCurrent(); // Remove from list
            got_changes = true;
        }
    }

    if (got_changes) OnSelectionChanged.Broadcast();
}

TArray<TWeakObjectPtr<ABaseUnit>> ASelectionManager::GetSelectedUnits() const
{
   return TArray(SelectedUnits);
}

void ASelectionManager::AddToSelection(AActor* entity)
{
    ABaseUnit* unit = Cast<ABaseUnit>(entity);

    if (unit) AddToSelection(unit);
}

void ASelectionManager::AddToSelection(ABaseUnit* unit)
{
    if (unit->EntityComponent->IsAlive() &&
        unit->EntityComponent->IsOwnedBy(OwnerID) &&
        !SelectedUnits.Contains(unit))
    {
        unit->EntityComponent->OnDieEntity.AddDynamic(this, &ASelectionManager::CheckSelection);
        unit->EntityComponent->OnDestroyEntity.AddDynamic(this, &ASelectionManager::CheckSelection);
        SelectedUnits.Add(unit);
        
        // // Make selection indicator
        // ASelectionIndicator* indicator = GetWorld()->SpawnActor<ASelectionIndicator>();
        // if (indicator)
        // {
        //     if (indicator->AttachToEntity(unit, OwnerID))
        //     {
        //         SelectedUnits.Add(unit, indicator);
        //     }
        // }
    }
}

void ASelectionManager::RemoveFromSelection(ABaseUnit* unit)
{
    if (!SelectedUnits.Contains(unit)) return;

    // Unsubscribe from die and destroy delegates
    // if (unit_w.IsValid())
    {
        unit->EntityComponent->OnDieEntity.RemoveDynamic(this, &ASelectionManager::CheckSelection);
        unit->EntityComponent->OnDestroyEntity.RemoveDynamic(this, &ASelectionManager::CheckSelection);
    }

	// TWeakObjectPtr<ASelectionIndicator> indicator = SelectedUnits[unit];

	// if (indicator.IsValid()) indicator->Destroy();
    SelectedUnits.Remove(unit);
}

void ASelectionManager::RemoveAllFromSelectionBut(ABaseUnit* unit)
{
    for (auto It = SelectedUnits.CreateIterator(); It; ++It)
    {
        TWeakObjectPtr<ABaseUnit> unitIt = *It;
        
        if ((!unitIt.IsValid() ||                          // If unit is destroyed
             !unitIt->EntityComponent->IsAlive() ||        // If unit is dead
             !unitIt->EntityComponent->IsOwnedBy(OwnerID)) // If is not owned by player
            && unit != unitIt.Get())
        {
            It.RemoveCurrent(); // Remove from list
        }
    }
}

void ASelectionManager::RemoveAllFromSelection()
{
  //   for (auto& Pair : SelectedUnits)
  //   {
		// if (Pair.Key.IsValid())
		// {
		// 	Pair.Key->EntityComponent->OnDieEntity.RemoveDynamic(this, &ASelectionManager::CheckSelection);
		// 	Pair.Key->EntityComponent->OnDestroyEntity.RemoveDynamic(this, &ASelectionManager::CheckSelection);
		// }
  //
  //       if (Pair.Value.IsValid())
  //           Pair.Value->Destroy();
  //   }

    SelectedUnits.Empty();
}
