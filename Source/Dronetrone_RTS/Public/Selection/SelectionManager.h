// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Units/BaseUnit.h"
#include "SelectionIndicator.h"

#include "SelectionManager.generated.h"

UCLASS()
class DRONETRONE_RTS_API ASelectionManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectionManager();

    void SelectUnit(ABaseUnit* Unit);

    void DeselectUnit(ABaseUnit* Unit);

    void DeselectAll();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
    UPROPERTY()
    TMap<TSoftObjectPtr<ABaseUnit>, TSoftObjectPtr<ASelectionIndicator>> SelectedUnits;

};
