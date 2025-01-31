// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
// Enumerator for player faction
enum class ERelationType : uint8 {
	OWN     = 0 UMETA(DisplayName = "Own"),
	FRIEND  = 1 UMETA(DisplayName = "Friend"),
	NEUTRAL = 2 UMETA(DisplayName = "Neutral"),
	FOE     = 3 UMETA(DisplayName = "Foe")
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
