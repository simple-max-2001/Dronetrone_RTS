// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UnitIconWidget.h"

void UUnitIconWidget::SetUnit(TWeakObjectPtr<ABaseUnit> NewUnit)
{
    if (Unit.IsValid()) OnRemoveDelegates.Broadcast();

    if (!NewUnit.IsValid()) return;
    
    Unit = NewUnit;
    OnCreateDelegates.Broadcast();
    CheckUnit();
}

void UUnitIconWidget::SetSelectionManager(TWeakObjectPtr<ASelectionManager> selection_manager)
{
    SelectionManager = selection_manager;
}

void UUnitIconWidget::CheckUnit()
{
    if (!Unit.IsValid() || !Unit->EntityComponent->IsAlive())
    {
        RemoveFromParent();
        Destruct();
    }
}

void UUnitIconWidget::NativeDestruct()
{
    Super::NativeDestruct();

    OnRemoveDelegates.Broadcast();
}

FReply UUnitIconWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    // Check if the left mouse button was pressed
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        if (SelectionManager.IsValid() && Unit.IsValid())
        {
            SelectionManager->Select(Unit);
            
            // Optionally, return a handled reply
            return FReply::Handled();
        }
    }

    // If not handled, return unhandled
    return FReply::Unhandled();
}
