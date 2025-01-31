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

void ASelectionManager::SelectUnit(ABaseUnit* Unit)
{
    if (!Unit || SelectedUnits.Contains(Unit))
        return;

    // Створюємо підсвітку, але тільки на клієнті
    ASelectionIndicator* Indicator = GetWorld()->SpawnActor<ASelectionIndicator>();
    if (Indicator)
    {
        Indicator->AttachToEntity(Unit);
        SelectedUnits.Add(Unit, Indicator);
    }
}

void ASelectionManager::DeselectUnit(ABaseUnit* Unit)
{
    if (!Unit || SelectedUnits.Contains(Unit))
        return;

    // Створюємо підсвітку, але тільки на клієнті
    ASelectionIndicator* Indicator = GetWorld()->SpawnActor<ASelectionIndicator>();
    if (Indicator)
    {
        Indicator->AttachToEntity(Unit);
        SelectedUnits.Add(Unit, Indicator);
    }
}

void ASelectionManager::DeselectAll()
{
    for (auto& Pair : SelectedUnits)
    {
        if (Pair.Value)
            Pair.Value->Destroy();
    }

    SelectedUnits.Empty();
}

