// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RTSGameState.generated.h"

/**
 * 
 */
UCLASS()
class DRONETRONE_RTS_API ARTSGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
    //UPROPERTY(ReplicatedUsing = OnRep_IsGamePaused)
    bool bIsGamePaused = false;

    UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
    void ServerSetPause(bool bPause);

protected:
    UFUNCTION()
    void OnRep_IsGamePaused();
};
