// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "SteeringMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DRONETRONE_RTS_API USteeringMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:
    virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Movement")
    float ForwardSpeed = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Movement")
    float RotationSpeed = 180.0f;
};
