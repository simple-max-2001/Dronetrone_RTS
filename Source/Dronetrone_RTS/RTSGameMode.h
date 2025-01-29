// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "RTSTypes.h"

#include "RTSGameMode.generated.h"

UCLASS(minimalapi)
class ARTSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARTSGameMode();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

    virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
	ERelationType GetRelation(EPlayerFaction own_faction, EPlayerFaction players_faction) const;

protected:
	virtual void BeginPlay();
};
