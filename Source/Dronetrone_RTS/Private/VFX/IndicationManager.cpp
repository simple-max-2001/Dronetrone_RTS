// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/IndicationManager.h"

// Sets default values
AIndicationManager::AIndicationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AIndicationManager::SetOwnerId(uint32 owner_id)
{
	OwnerID = owner_id;
}

void AIndicationManager::SetSelectionManager(TWeakObjectPtr<ASelectionManager> selection_manager)
{
	SelectionManager = selection_manager;
}

void AIndicationManager::UpdateSelectionIndicators()
{
	if (!SelectionManager.IsValid()) return;
	
	const TArray<TWeakObjectPtr<ABaseUnit>>& SelectedUnits = SelectionManager->GetSelectedUnits();
	
	ClearIndicators();
	
	if (SelectedUnits.IsEmpty()) return;

	for (TWeakObjectPtr unit : SelectedUnits)
	{
		if (unit.IsValid())
		{
			// Make selection indicator
			ASelectionIndicator* indicator = GetWorld()->SpawnActor<ASelectionIndicator>();
			if (indicator)
			{
				if (indicator->AttachToEntity(TSoftObjectPtr<ABaseUnit>(unit.Get()), OwnerID))
			        ActiveIndicators.Add(unit, indicator);
			}
		}
	}
}

// Called when the game starts or when spawned
void AIndicationManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AIndicationManager::ClearIndicators()
{
	if (ActiveIndicators.IsEmpty()) return;
	
	for (auto It = ActiveIndicators.CreateIterator(); It; ++It)
	{
		if (!It.Value().IsValid()) continue;	// Indicator is not valid, skip it
		It.Value()->Destroy();
	}

	ActiveIndicators.Empty();
}

