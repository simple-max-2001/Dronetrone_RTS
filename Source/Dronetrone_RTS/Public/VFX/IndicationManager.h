// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SelectionIndicator.h"
#include "Selection/SelectionManager.h"
#include "Units/BaseUnit.h"

#include "IndicationManager.generated.h"

UCLASS()
class DRONETRONE_RTS_API AIndicationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIndicationManager();

	void SetOwnerId(uint32 owner_id);
	
	void SetSelectionManager(TWeakObjectPtr<ASelectionManager> selection_manager);

	// Update indicators for selected units
	UFUNCTION()
	void UpdateSelectionIndicators();

	// Clear all indicators
	void ClearIndicators();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<TWeakObjectPtr<ABaseUnit>, TWeakObjectPtr<ASelectionIndicator>> ActiveIndicators;

	UPROPERTY()
	TWeakObjectPtr<ASelectionManager> SelectionManager;
	
	int32 OwnerID = -1;
};
