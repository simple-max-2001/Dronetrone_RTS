// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TimerManager.h"

#include "Units/BaseUnit.h"

#include "RTSGameState.generated.h"


UCLASS()
class DRONETRONE_RTS_API ARTSGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    void UpdateUnits();

    TArray<TSoftObjectPtr<ABaseUnit>> GetAllUnits();

    //UPROPERTY(ReplicatedUsing = OnRep_IsGamePaused)
    bool bIsGamePaused = false;

    UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
    void ServerSetPause(bool bPause);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entities")
    TArray<TSoftObjectPtr<AActor>> Buildings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entities")
    TArray<TSoftObjectPtr<ABaseUnit>> Units;

    UFUNCTION()
    void OnRep_IsGamePaused();

    FTimerHandle UnitsCheckingHandle;
};
