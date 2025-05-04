// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "Buildings/BaseBuilding.h"
#include "Units/BaseUnit.h"
#include "Components/Comm/CommModuleComponent.h"

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
	
	void UpdateEntities();

	int32 GetOwnerID() const
	{
		return OwnerID;
	}

	TArray<TSoftObjectPtr<ABaseUnit>> GetAllUnits() const
	{
		return TArray(Units);
	}

	TArray<TSoftObjectPtr<ABaseBuilding>> GetAllBuildings() const
	{
		return TArray(Buildings);
	}

protected:
	//UPROPERTY(ReplicatedUsing=OnRep_OwnedUnits)
	UPROPERTY()
	TArray<TSoftObjectPtr<ABaseUnit>> Units;
	
	UPROPERTY()
	TArray<TSoftObjectPtr<ABaseBuilding>> Buildings;

    UPROPERTY()
	int32 OwnerID;

	UPROPERTY()
	float UpdatePeriod = .3f;

	FTimerHandle UpdateEntitiesHandle;
};
