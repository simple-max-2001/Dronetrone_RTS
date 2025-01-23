// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
// Enumerator for armor type
enum class EOwnership : uint8 {
	NEUTRAL = 0 UMETA(DisplayName = "Neutral"),
	PLAYER_1 = 1 UMETA(DisplayName = "Player 1"),
	PLAYER_2 = 2 UMETA(DisplayName = "Player 2"),
	PLAYER_3 = 3 UMETA(DisplayName = "Player 3"),
	PLAYER_4 = 4 UMETA(DisplayName = "Player 4"),
	SERVER = 5 UMETA(DisplayName = "Server")
};

class DRONETRONE_RTS_API ABaseUnit;

/**
 * 
 */
//class DRONETRONE_RTS_API RTSTypes
//{
//public:
//	RTSTypes();
//	~RTSTypes();
//};
