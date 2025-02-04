// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FastPhysicsEngine.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API UFastPhysicsEngine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFastPhysicsEngine();

	void SetPawn(APawn* pawn);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetDesiredVelocity(FVector Velocity);

	// Set desired yaw rate for controller
	void SetDesiredYawRate(float YawRate);

protected:

	// Apply unit change of velocity
	void ApplyMovement(float DeltaTime);

	// Apply gravity and check collisions under wheels
	void CheckCollisions(float DeltaTime);

	// Draw debug lines to unit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	bool bDrawDebug = false;

	// Apply gravity to unit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	bool bUseGravity = true;

	// Minimum required distance between wheel and ground, cm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	float RequiredClearance = 2.f;

	// Applied acceleration during fall, cm*s^-2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	float FallAcceleration = 981.f;

	// forward wheel position, cm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	float ForwardOffset = 100.f;

	// rightward wheel position, cm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	float RightOffset = 100.f;

	// maximum allowed velocity, cm/s
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSpeed = 5000.f;

	// maximum allowed yaw rate, deg/s
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxYawRate = 720.f;

	// Interpolation velocity (time to change velocity from current to desired is 3/InterpVel), s
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float InterpVelocity = 6.f;

	// Current fall speed for gravity, cm/s
	float FallSpeed = .0f;

	// If vehicle is grounded
	bool bIsGrounded = true;

	// desired unit velocity, cm/s
	FVector DesiredVelocity_ = FVector::ZeroVector;
	// current unit velocity, cm/s
	FVector CurrentVelocity_ = FVector::ZeroVector;

	// desired yaw rate, deg/s
	float DesiredYawRate_ = .0f;

	APawn* Pawn;
};
