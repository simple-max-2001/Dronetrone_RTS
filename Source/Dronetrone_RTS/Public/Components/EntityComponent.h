// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "EntityComponent.generated.h"

class UEntityComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnEntity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDieEntity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroyEntity);


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API UEntityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEntityComponent();

	UFUNCTION(BlueprintCallable)
	void KillEntity();

	UFUNCTION(BlueprintCallable)
	void DestroyEntity();

	UPROPERTY(BlueprintAssignable)
	FSpawnEntity OnSpawnEntity;

	UPROPERTY(BlueprintAssignable)
	FDieEntity OnDieEntity;

	UPROPERTY(BlueprintAssignable)
	FDieEntity OnDestroyEntity;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsAlive();

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsAlive = true;
		
};
