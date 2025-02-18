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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void SetOwnerID(int32 owner_id);
	void SetKeepSelection(bool keep_selection = false);

	void Select(TWeakObjectPtr<AActor> entity);
	void Select(TArray<AActor*> entities);
	void Select(TArray<TWeakObjectPtr<AActor>> entities);

	void Select(TWeakObjectPtr<ABaseUnit> unit);
	void Select(TArray<ABaseUnit*> units);
	void Select(TArray<TWeakObjectPtr<ABaseUnit>> units);

    void DeselectAll();

	UFUNCTION()
	void CheckSelection();

	UFUNCTION()
	TArray<TWeakObjectPtr<ABaseUnit>> GetSelectedUnits() const;

	UPROPERTY()
	FOnSelectionChanged OnSelectionChanged;

protected:

	// Add one entity to selection
	void AddToSelection(AActor* entity);
	void AddToSelection(ABaseUnit* entity);

	// Remove one entity from selection
	void RemoveFromSelection(ABaseUnit* unit);

	// Remove all entities from selection excepting one
	void RemoveAllFromSelectionBut(ABaseUnit* unit);

	// Remove all entities from selection
	void RemoveAllFromSelection();

    UPROPERTY()
    TArray<TWeakObjectPtr<ABaseUnit>> SelectedUnits;

	int32 OwnerID = -1;
	bool bKeepSelection = false;
};
