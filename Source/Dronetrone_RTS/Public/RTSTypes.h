// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
// Enumerator for player faction
enum class ERelationType : uint8 {
	Own     = 0 UMETA(DisplayName = "Own"),
	Friend  = 1 UMETA(DisplayName = "Friend"),
	Neutral = 2 UMETA(DisplayName = "Neutral"),
	Foe     = 3 UMETA(DisplayName = "Foe")
};

// Get entity component 
#define GET_ENTITY(actor) actor->FindComponentByClass<UEntityComponent>() 
