// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

#include "Selection/SelectionManager.h"
#include "Units/BaseUnit.h"

#include "UnitIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRONETRONE_RTS_API UUnitIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void SetUnit(TSoftObjectPtr<ABaseUnit> unit);

    void SetSelectionManager(ASelectionManager* selection_manager);

    /** Оновлення інформації про юніта */
	UFUNCTION()
    void UpdateUnitInfo();

protected:

	virtual void NativeDestruct() override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

    UPROPERTY(meta = (BindWidget))
    UImage* UnitImage;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<ABaseUnit> Unit;

    UPROPERTY(BlueprintReadOnly)
	ASelectionManager* SelectionManager;
};
