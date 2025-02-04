// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UnitIconWidget.h"

void UUnitIconWidget::SetUnit(TSoftObjectPtr<ABaseUnit> unit)
{
    Unit = unit;
    if (Unit.IsValid())
    {
        Unit->HealthComponent->OnHealthChanged.AddDynamic(this, &UUnitIconWidget::UpdateUnitInfo);
    }

    UpdateUnitInfo();
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