// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"

#include "UnitIconWidget.h"
#include "Selection/SelectionManager.h"

#include "SelectionPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRONETRONE_RTS_API USelectionPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetSelectionManager(TWeakObjectPtr<ASelectionManager> selection_manager);

    UFUNCTION(BlueprintCallable)
    void UpdateSelection();

protected:

    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* UnitGridPanel;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ASelectionManager> SelectionManager;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUnitIconWidget> UnitIconWidgetClass;
	
};
