// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LevelManager.h"
#include "CameraPawn.generated.h"

UCLASS()
class DRONETRONE_RTS_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MovePawn(float forward_axis, float right_axis);

	void ChangeZoom(float axis);

	void PanCamera(float axis_x, float axis_y);

	void SetSpeedUp(bool bInSpeedUp);

	/* Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/* Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/* Collision sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionSphere;

	/* Maximum spring arm length */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings|Zoom", meta = (AllowPrivateAccess = "true"))
	float MaxArmLength = 2400.f;

	/* Minimum spring arm length */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float MinArmLength = 1000.f;

	/* Maximum spring arm length */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float MaxPitch = 70.f;

	/* Minimum spring arm length */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float MinPitch = 20.f;

	/* Movement speed of camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 800.f;

	/* Yaw speed of camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float YawSpeed = 3600.f;

	/* Pitch speed of camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float PitchSpeed = 1800.f;

	/* Movement speed of camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float DeltaZoom = .05f;

	/* High multiplier of movement speed of camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float MaxMoveSpeedMultiplier = 2.5f;

	/* High multiplier of movement speed of camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float TickTime = .02f;

private:

	bool bSpeedUp = false;
	float ZoomPosition = 1;

	ALevelManager* LevelManager;
};
