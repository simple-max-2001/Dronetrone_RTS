// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CommModuleComponent.generated.h"

class UCommRelayComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API UCommModuleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCommModuleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable)
	bool IsConnected() const;
	
	UFUNCTION(BlueprintCallable)
	void UpdateConnection();

protected:
	bool bIsConnected = false;

	TWeakObjectPtr<UCommRelayComponent> CurrentRelay;

};
