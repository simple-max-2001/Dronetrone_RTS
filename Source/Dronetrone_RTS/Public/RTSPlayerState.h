// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "RTSTypes.h"

#include "RTSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DRONETRONE_RTS_API ARTSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    ARTSPlayerState();

    // Player faction for ownership
    //UPROPERTY(ReplicatedUsing=OnRep_TeamID)

    void Setup(EPlayerFaction player_faction);

    /** Додає юніт до списку власності */
    //void AddUnit(AUnitBase* Unit);

    /** Видаляє юніт зі списку власності */
    //void RemoveUnit(AUnitBase* Unit);
	
	void UpdateUnits();

	TArray<TSoftObjectPtr<ABaseUnit>> GetAllUnits();


protected:
    //UPROPERTY(ReplicatedUsing=OnRep_OwnedUnits)
    UPROPERTY()
    TArray<TSoftObjectPtr<ABaseUnit>> Units;

    UPROPERTY()
	EPlayerFaction PlayerFaction;
};
