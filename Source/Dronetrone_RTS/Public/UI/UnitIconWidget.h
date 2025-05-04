// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Selection/SelectionManager.h"
#include "Units/BaseUnit.h"

#include "UnitIconWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCreateDelegates);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRemoveDelegates);

/**
 * 
 */
UCLASS()
class DRONETRONE_RTS_API UUnitIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void SetUnit(TWeakObjectPtr<ABaseUnit> NewUnit);

    void SetSelectionManager(TWeakObjectPtr<ASelectionManager> selection_manager);

    /** Оновлення інформації про юніта */
	UFUNCTION(BlueprintCallable)
    void CheckUnit();

protected:
	virtual void NativeDestruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ABaseUnit> Unit;

	UPROPERTY(BlueprintAssignable)
	FCreateDelegates OnCreateDelegates;

	UPROPERTY(BlueprintAssignable)
	FRemoveDelegates OnRemoveDelegates;

    UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ASelectionManager> SelectionManager;
};
