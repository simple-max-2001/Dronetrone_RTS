// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "RTSTypes.h"
#include "Units/BaseUnit.h"

#include "RTSGameState.generated.h"


USTRUCT(BlueprintType)
struct FGroupInfo
{
    GENERATED_USTRUCT_BODY()

    // List of units in group
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Assets")
    TArray<TSoftObjectPtr<ABaseUnit>> Units;

    // Leader of group
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Assets")
    TSoftObjectPtr<ABaseUnit> Leader;

    FGroupInfo() {}

    FGroupInfo(TArray<TSoftObjectPtr<ABaseUnit>> units, int32 leader_id = 0)
    {
        Units = TArray<TSoftObjectPtr<ABaseUnit>>(units);

        if (leader_id < 0 || leader_id >= Units.Num()) leader_id = 0;

        Leader = Units[leader_id];
    }
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
    GENERATED_USTRUCT_BODY()

    // Ownership number
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ownership")
    EOwnership PlayerID = EOwnership::PLAYER_1;

    // If player is human
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
    bool bIsHuman = true;

    // Name of player
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FString PlayerName;

    // List of player's buildings (Soft Object Reference for optimization)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ownership")
    TArray<TSoftObjectPtr<AActor>> Buildings;

    // List of player's units
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ownership")
    TArray<TSoftObjectPtr<ABaseUnit>> Units;

    // List of groups
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ownership")
    TMap<int32, FGroupInfo> UnitGroups;

    FPlayerInfo(EOwnership player_id = EOwnership::PLAYER_1, bool is_human = true)
    {
        PlayerID = player_id;
    }

    // Add new building
    void AddBuilding(AActor* NewBuilding)
    {
        Buildings.Add(NewBuilding);
    }

    // Add new units
    void AddUnit(TSoftObjectPtr<ABaseUnit> NewUnit)
    {
        Units.Add(NewUnit);
    }

    // Додавання групи юнітів
    void CreateUnitGroup(int32 GroupID, const TArray<TSoftObjectPtr<ABaseUnit>> GroupUnits)
    {
        FGroupInfo group_info(GroupUnits);
        UnitGroups.Add(GroupID, group_info);
    }

};

/**
 * 
 */
UCLASS()
class DRONETRONE_RTS_API ARTSGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    void AddPlayerInfo(FPlayerInfo player_info);

    FPlayerInfo* GetPlayerInfo();

    void UpdateUnits();

    void UpdatePlayersUnits(EOwnership player);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
    TArray<TSoftObjectPtr<AActor>> Buildings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
    TArray<TSoftObjectPtr<ABaseUnit>> Units;

    //UPROPERTY(ReplicatedUsing = OnRep_IsGamePaused)
    bool bIsGamePaused = false;

    UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
    void ServerSetPause(bool bPause);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Players")
    TMap<EOwnership, FPlayerInfo> Players;

    UFUNCTION()
    void OnRep_IsGamePaused();
};
