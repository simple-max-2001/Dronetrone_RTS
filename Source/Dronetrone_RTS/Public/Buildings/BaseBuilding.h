// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

#include "Components/EntityComponent.h"
#include "Components/HealthComponent.h"

#include "BaseBuilding.generated.h"

UCLASS()
class DRONETRONE_RTS_API ABaseBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxComponent;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UEntityComponent* EntityComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
