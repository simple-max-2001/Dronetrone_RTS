// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSHUD.h"
#include "Units/BaseUnit.h"

void ARTSHUD::BeginPlay()
{
    Super::BeginPlay();
}

void ARTSHUD::SetSelectionManager(ASelectionManager* selection_manger)
{
    SelectionManager = selection_manger;
}

void ARTSHUD::StartSelection()
{
    if (APlayerController* PC = GetOwningPlayerController())
    {
        bIsSelecting = true;

        PC->GetMousePosition(SelectionStart.X, SelectionStart.Y);
        SelectionEnd = SelectionStart;
    }
}

void ARTSHUD::UpdateSelection()
{
    if (bIsSelecting)
    {
        if (APlayerController* PC = GetOwningPlayerController())
        {
            PC->GetMousePosition(SelectionEnd.X, SelectionEnd.Y);
        }
    }
}

void ARTSHUD::EndSelection()
{
    bIsSelecting = false;
    SelectUnitsInRectangle();
}

void ARTSHUD::SelectUnitsInRectangle()
{
    if (SelectionStart.Equals(SelectionEnd, 5.0f)) // If it is only click
    {
        return;
    }

    APlayerController* PC = GetOwningPlayerController();
    if (!PC) return;

    TArray<ABaseUnit*> SelectedUnits;

    // Receive all units in rectangle
    GetActorsInSelectionRectangle<ABaseUnit>(SelectionStart, SelectionEnd, SelectedUnits, false, false);

    // Send selection to SelectionManager
    if (SelectionManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("SelectionManager sent %d"), SelectedUnits.Num());
        SelectionManager->SelectUnits(SelectedUnits);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No SelectionManager"));
    }
}

void ARTSHUD::DrawHUD()
{
    Super::DrawHUD();

    if (bIsSelecting) // If selecting is active
    {
        UpdateSelection();

        float Width = SelectionEnd.X - SelectionStart.X;
        float Height = SelectionEnd.Y - SelectionStart.Y;

        // Встановлюємо колір та альфа-канал
        FLinearColor BoxColor = FLinearColor(0, 1.f, .5f, 0.3f);
        FLinearColor ContourColor = FLinearColor(0, 5.f, 0, 0.6f);

        // Малюємо напівпрозорий прямокутник
        DrawRect(BoxColor, SelectionStart.X, SelectionStart.Y, Width, Height);

        // Малюємо контур
        DrawLine(SelectionStart.X, SelectionStart.Y, SelectionEnd.X, SelectionStart.Y, ContourColor);
        DrawLine(SelectionStart.X, SelectionEnd.Y, SelectionEnd.X, SelectionEnd.Y, ContourColor);
        DrawLine(SelectionStart.X, SelectionStart.Y, SelectionStart.X, SelectionEnd.Y, ContourColor);
        DrawLine(SelectionEnd.X, SelectionStart.Y, SelectionEnd.X, SelectionEnd.Y, ContourColor);
    }
}