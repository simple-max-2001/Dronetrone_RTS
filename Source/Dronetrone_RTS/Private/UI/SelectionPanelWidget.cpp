// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SelectionPanelWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Overlay.h"

void USelectionPanelWidget::SetSelectionManager(TWeakObjectPtr<ASelectionManager> selection_manager)
{
    // Remove previous delegate if exists
    if (SelectionManager.IsValid())
        SelectionManager->OnSelectionChanged.RemoveDynamic(this, &USelectionPanelWidget::UpdateSelection);

    // Add new delegate
    if (selection_manager.IsValid())
    {
        SelectionManager = selection_manager;
        SelectionManager->OnSelectionChanged.AddDynamic(this, &USelectionPanelWidget::UpdateSelection);
    }
}

void USelectionPanelWidget::UpdateSelection()
{
    if (!SelectionManager.IsValid()) return;

    if (!UnitGridPanel) return;

    UnitGridPanel->ClearChildren();
    TArray<TWeakObjectPtr<ABaseUnit>> SelectedUnits = SelectionManager->GetSelectedUnits();

    for (int32 i = 0; i < SelectedUnits.Num(); i++)
    {
        UUnitIconWidget* UnitIcon = CreateWidget<UUnitIconWidget>(this, UnitIconWidgetClass);
        if (UnitIcon)
        {
            UnitIcon->SetUnit(SelectedUnits[i]);
            UnitIcon->SetSelectionManager(SelectionManager);
            UnitGridPanel->AddChildToUniformGrid(UnitIcon, i / 4, i % 4);
        }
    }
}
