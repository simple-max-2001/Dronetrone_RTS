// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RTSGameMode.generated.h"

UCLASS(minimalapi)
class ARTSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARTSGameMode();

protected:
	virtual void BeginPlay();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
