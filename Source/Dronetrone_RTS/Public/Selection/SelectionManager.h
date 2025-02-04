// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SelectionIndicator.h"
#include "Units/BaseUnit.h"

#include "SelectionManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectionChanged);

UCLASS()
class DRONETRONE_RTS_API ASelectionManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectionManager();

	void SetOwnerID(int32 owner_id);
	void SetKeepSelection(bool keep_selection = false);

	void SelectUnit(TSoftObjectPtr<ABaseUnit> unit, bool deselect = true, bool broadcast = true);

	void SelectUnits(TArray<ABaseUnit*> units, bool broadcast = true);
	void SelectUnits(TArray<TSoftObjectPtr<ABaseUnit>> units, bool broadcast = true);

    void DeselectUnit(TSoftObjectPtr<ABaseUnit> unit, bool broadcast = true);

    void DeselectAll(bool broadcast = true);

	UFUNCTION()
	void CheckSelection();

	UFUNCTION()
	TArray<TSoftObjectPtr<ABaseUnit>> GetSelectedUnits();

	UPROPERTY(BlueprintAssignable)
	FOnSelectionChanged OnSelectionChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
    UPROPERTY()
    TMap<TSoftObjectPtr<ABaseUnit>, TSoftObjectPtr<ASelectionIndicator>> SelectedUnits;

	int32 OwnerID = -1;
	bool bKeepSelection = false;
};
