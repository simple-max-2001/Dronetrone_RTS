// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

UCLASS()
class DRONETRONE_RTS_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool isAllowedCameraZone(FVector new_location);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBox2D> AllowedCameraZones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBox2D> AllowedBuildZones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRestrictCameraMovement = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRestrictBuilding = false;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void DrawAllowedZones();
#endif
};
