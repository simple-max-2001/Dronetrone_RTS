// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"

#include "ControlComponent.h"

#include "Components/EntityComponent.h"
#include "Components/HealthComponent.h"
#include "Components/Comm/CommModuleComponent.h"

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxComponent;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UEntityComponent* EntityComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UControlComponent* ControlComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCommModuleComponent* CommsComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UFastPhysicsEngine* FastPhysicsEngine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* IconTexture;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetUnitIcon();

private:
	UFUNCTION()
	void OnUnitDeath();

};
