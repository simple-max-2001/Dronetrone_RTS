// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"

#include "Components/Comm/CommRelayComponent.h"
#include "CommManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNetworkCheckEvent);

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API UCommManagerComponent : public UCommRelayComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void CheckConnection() const;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnNetworkCheckEvent OnNetworkCheckStarted;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnNetworkCheckEvent OnNetworkCheckFinished;

protected:
	FTimerHandle NetworkCheckingHandle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float NetworkCheckingInterval = .5f;
};
