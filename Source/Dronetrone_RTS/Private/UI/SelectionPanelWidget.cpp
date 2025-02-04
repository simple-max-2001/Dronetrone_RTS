// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SelectionPanelWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Overlay.h"

void USelectionPanelWidget::SetSelectionManager(ASelectionManager* selection_manager)
{
    // Remove previous delegate if exists
    if (SelectionManager) SelectionManager->OnSelectionChanged.RemoveDynamic(this, &USelectionPanelWidget::UpdateSelection);

    SelectionManager = selection_manager;

    // Add new delegate
    if (SelectionManager) SelectionManager->OnSelectionChanged.AddDynamic(this, &USelectionPanelWidget::UpdateSelection);
}

void USelectionPanelWidget::UpdateSelection()
{
    if (!SelectionManager) return;

    if (!UnitGridPanel) return;

    UnitGridPanel->ClearChildren();
    TArray<TSoftObjectPtr<ABaseUnit>> SelectedUnits = SelectionManager->GetSelectedUnits();

    for (int32 i = 0; i < SelectedUnits.Num(); i++)
    {
        UUnitIconWidget* UnitIcon = CreateWidget<UUnitIconWidget>(this, UnitIconWidgetClass);
        if (UnitIcon)
        {
            UnitIcon->SetUnit(SelectedUnits[i]);
            UnitGridPanel->AddChildToUniformGrid(UnitIcon, i / 4, i % 4);
        }
    }
}
