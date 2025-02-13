// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSHUD.h"
#include "Units/BaseUnit.h"

#include "Kismet/GameplayStatics.h"

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
        bSelectionComplete = false;
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
    bSelectionComplete = true;
}

void ARTSHUD::SelectUnitsInRectangle()
{
    if (!SelectionManager) 
    {
        UE_LOG(LogTemp, Warning, TEXT("No SelectionManager"));
        return;
    }

    if (SelectionStart.Equals(SelectionEnd, 5.0f)) // If it is only click
    {
        return;
    }

    APlayerController* PC = GetOwningPlayerController();
    if (!PC) return;

    TArray<ABaseUnit*> SelectedUnits;

    // Receive all units in rectangle
    bool res = GetActorsInSelectionRectangle<ABaseUnit>(SelectionStart, SelectionEnd, SelectedUnits, false, false);
    UE_LOG(LogTemp, Warning, TEXT("SelectionManager sent (%d): %d"), res, SelectedUnits.Num());
    
    SelectionManager->Select(SelectedUnits);

}

void ARTSHUD::DrawHUD()
{
    Super::DrawHUD();

    if (bIsSelecting) // If selecting is active
    {
        // Update current selection box
        UpdateSelection();

        // If selection complete
        if (bSelectionComplete)
        {
            SelectUnitsInRectangle();
            bSelectionComplete = false;
            bIsSelecting = false;
            return;
        }

        float Width = SelectionEnd.X - SelectionStart.X;
        float Height = SelectionEnd.Y - SelectionStart.Y;

        FLinearColor BoxColor = FLinearColor(0, 1.f, .5f, 0.3f);
        FLinearColor ContourColor = FLinearColor(0, 5.f, 0, 0.6f);

        // Draw filling of selection rectangle
        DrawRect(BoxColor, SelectionStart.X, SelectionStart.Y, Width, Height);

        // Draw contour of selection rectangle
        DrawLine(SelectionStart.X, SelectionStart.Y, SelectionEnd.X, SelectionStart.Y, ContourColor);
        DrawLine(SelectionStart.X, SelectionEnd.Y, SelectionEnd.X, SelectionEnd.Y, ContourColor);
        DrawLine(SelectionStart.X, SelectionStart.Y, SelectionStart.X, SelectionEnd.Y, ContourColor);
        DrawLine(SelectionEnd.X, SelectionStart.Y, SelectionEnd.X, SelectionEnd.Y, ContourColor);
    }
}