// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UnitIconWidget.h"

void UUnitIconWidget::SetUnit(TSoftObjectPtr<ABaseUnit> unit)
{
    if (Unit.IsValid()) Unit->HealthComponent->OnHealthChanged.RemoveDynamic(this, &UUnitIconWidget::UpdateUnitInfo);

    Unit = unit;

    if (Unit.IsValid()) Unit->HealthComponent->OnHealthChanged.AddDynamic(this, &UUnitIconWidget::UpdateUnitInfo);

    UpdateUnitInfo();
}

void UUnitIconWidget::SetSelectionManager(ASelectionManager* selection_manager)
{
    SelectionManager = selection_manager;
}

void UUnitIconWidget::UpdateUnitInfo()
{
    if (!Unit.IsValid()) RemoveFromParent();

    // Update unit icon
    UTexture2D* unit_icon = Unit->GetUnitIcon();
    if (UnitImage && unit_icon)
    {
        UnitImage->SetBrushFromTexture(unit_icon);
    }

    // Update unit health bar
    if (HealthBar) HealthBar->SetPercent(Unit->HealthComponent->GetHealthPercentage());
}

void UUnitIconWidget::NativeDestruct()
{
    Super::NativeDestruct();
    
    if (Unit.IsValid()) Unit->HealthComponent->OnHealthChanged.RemoveDynamic(this, &UUnitIconWidget::UpdateUnitInfo);
}

FReply UUnitIconWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    // Check if the left mouse button was pressed
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        // Handle the left mouse button down event
        UE_LOG(LogTemp, Warning, TEXT("Left mouse button down!"));

        if (SelectionManager && Unit.IsValid())
        {
            SelectionManager->SelectUnit(Unit);
            
            // Optionally, return a handled reply
            return FReply::Handled();
        }
    }

    // If not handled, return unhandled
    return FReply::Unhandled();
}