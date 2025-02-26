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

    void Setup(int32 owner_id);
	
	void UpdateUnits();

	int32 GetOwnerID() const;

	TArray<TSoftObjectPtr<ABaseUnit>> GetAllUnits();

	TArray<TSoftObjectPtr<ABaseUnit>> GetAllUnits() const;

protected:
    //UPROPERTY(ReplicatedUsing=OnRep_OwnedUnits)
    UPROPERTY()
    TArray<TSoftObjectPtr<ABaseUnit>> Units;

    UPROPERTY()
	int32 OwnerID;
};
