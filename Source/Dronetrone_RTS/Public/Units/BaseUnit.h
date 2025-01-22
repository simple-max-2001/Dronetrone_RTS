// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/EntityComponent.h"
#include "Components/HealthComponent.h"
#include "Physics/FastPhysicsEngine.h"
#include "BaseUnit.generated.h"

UCLASS()
class DRONETRONE_RTS_API ABaseUnit : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UEntityComponent* EntityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UFastPhysicsEngine* FastPhysicsEngine;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
