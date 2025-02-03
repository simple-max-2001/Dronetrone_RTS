// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Selection/SelectionManager.h"

#include "RTSHUD.generated.h"

/**
 * 
 */
UCLASS()
class DRONETRONE_RTS_API ARTSHUD : public AHUD
{
	GENERATED_BODY()

public:
    virtual void DrawHUD() override;

    virtual void BeginPlay() override;
    void SetSelectionManager(ASelectionManager* selection_manger);

    void StartSelection();
    void UpdateSelection();
    void EndSelection();

    void SelectUnitsInRectangle();

private:

    bool bIsSelecting = false;
    bool bSelectionComplete = false;
    FVector2D SelectionStart;
    FVector2D SelectionEnd;

    ASelectionManager* SelectionManager;

};
