// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AntennaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API UAntennaComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAntennaComponent();
	
	// Get antenna gain depending on direction, dBi
	virtual float GetAntennaGain(FVector Direction) const;

protected:
	// Maximum antenna gain, dBi
	UPROPERTY(EditDefaultsOnly, Category = "Antenna")
	float MaxAntennaGain = 0.f;
};
