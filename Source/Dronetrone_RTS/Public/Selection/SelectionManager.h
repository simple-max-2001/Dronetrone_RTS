// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

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

	void SetOwnerID(int32 Owner_ID);
	void SetKeepSelection(bool bKeep_Selection = false);

	void Select(TWeakObjectPtr<AActor> Entity);
	void Select(TArray<AActor*> Entities);
	void Select(TArray<TWeakObjectPtr<AActor>> Entities);

	void Select(TWeakObjectPtr<ABaseUnit> Unit);
	void Select(TArray<ABaseUnit*> Units);
	void Select(TArray<TWeakObjectPtr<ABaseUnit>> Units);

    void DeselectAll();

	UFUNCTION()
	void CheckSelection();

	UFUNCTION()
	TArray<TWeakObjectPtr<ABaseUnit>> GetSelectedUnits() const;

	UPROPERTY()
	FOnSelectionChanged OnSelectionChanged;

protected:

	// Add one entity to selection
	void AddToSelection(AActor* Entity);
	void AddToSelection(ABaseUnit* Unit);

	// Remove one entity from selection
	void RemoveFromSelection(ABaseUnit* Unit);

	// Remove all entities from selection excepting one
	void RemoveAllFromSelectionBut(ABaseUnit* Unit);

	// Remove all entities from selection
	void RemoveAllFromSelection();

    UPROPERTY()
    TArray<TWeakObjectPtr<ABaseUnit>> SelectedUnits;

	int32 OwnerID = -1;
	bool bKeepSelection = false;
};
