// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "Buildings/BaseBuilding.h"
#include "Units/BaseUnit.h"
#include "Components/Comm/CommRelayComponent.h"

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

    void Setup(int32 Owner_ID);
	
	void UpdateUnits();

	int32 GetOwnerID() const;

	TArray<TSoftObjectPtr<ABaseUnit>> GetAllUnits();

	TArray<TSoftObjectPtr<ABaseUnit>> GetAllUnits() const;

protected:
	//UPROPERTY(ReplicatedUsing=OnRep_OwnedUnits)
	UPROPERTY()
	TArray<TSoftObjectPtr<ABaseUnit>> Units;
	
	UPROPERTY()
	TArray<TSoftObjectPtr<ABaseBuilding>> Buildings;
	
	UPROPERTY()
	TArray<TSoftObjectPtr<UCommRelayComponent>> Relays;

    UPROPERTY()
	int32 OwnerID;

	UPROPERTY()
    FTimerHandle NetworkUpdateHandle;
};
