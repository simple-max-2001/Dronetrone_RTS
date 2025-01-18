// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelManager.h"
#include "RTSUtils.generated.h"

/**
 * 
 */
UCLASS()
class DRONETRONE_RTS_API URTSUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = Manager)
	static ALevelManager* GetLevelManager(const UWorld* world);

};
