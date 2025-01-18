// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "RTSUtils.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create collision sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(16.f);
	CollisionSphere->PrimaryComponentTick.bTickEvenWhenPaused = true;
	CollisionSphere->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	CollisionSphere->SetCollisionProfileName(FName("CameraPawn"));

	// Create a camera boom...
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CollisionSphere);
	//SpringArm->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	SpringArm->TargetArmLength = MaxArmLength;
	SpringArm->SetRelativeRotation(FRotator(-MaxPitch, 0.f, 0.f));
	SpringArm->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	SpringArm->PrimaryComponentTick.bTickEvenWhenPaused = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	//CameraComponent->SetupAttachment(CollisionSphere);
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	CameraComponent->PrimaryComponentTick.bTickEvenWhenPaused = true;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();

	LevelManager = URTSUtils::GetLevelManager(GetWorld());

	if (LevelManager)
	{
		UE_LOG(LogTemp, Log, TEXT("LevelManager"));
	}
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACameraPawn::MovePawn(float forward_axis, float right_axis)
{
	//UE_LOG(LogTemp, Log, TEXT("MovePawn"));
	float axis_length = sqrtf(forward_axis * forward_axis + right_axis * right_axis);

	if (axis_length > 1)
	{
		forward_axis /= axis_length;
		right_axis /= axis_length;
	}

	float SpeedMult = 1.f;
	if (bSpeedUp) SpeedMult = MaxMoveSpeedMultiplier;

	FVector location = GetActorLocation();
	FVector delta_location = FVector(forward_axis, right_axis, .0f) * MoveSpeed * SpeedMult * GetWorld()->GetDeltaSeconds();

	location += delta_location.RotateAngleAxis(GetActorRotation().Yaw, FVector(.0f, .0f, 1.f));

	if (LevelManager)
	{
		if (!LevelManager->isAllowedCameraZone(location)) return;
	}

	SetActorLocation(location, true);
}

void ACameraPawn::ChangeZoom(float axis)
{
	//UE_LOG(LogTemp, Log, TEXT("ChangeZoom"));
	ZoomPosition += DeltaZoom * axis;

	if (ZoomPosition < 0) ZoomPosition = 0;
	else if (ZoomPosition > 1) ZoomPosition = 1;

	SpringArm->TargetArmLength = MinArmLength + ZoomPosition * (MaxArmLength - MinArmLength);
}

void ACameraPawn::PanCamera(float axis_x, float axis_y)
{
	//UE_LOG(LogTemp, Log, TEXT("PanCamera"));
	float pitch = -SpringArm->GetTargetRotation().Pitch - axis_y * PitchSpeed;
	float d_yaw = axis_x * YawSpeed;

	if (pitch > MaxPitch) pitch = MaxPitch;
	else if (pitch < MinPitch) pitch = MinPitch;

	AddActorWorldRotation(FRotator(.0f, d_yaw, .0f), true);
	SpringArm->SetRelativeRotation(FRotator(-pitch, .0f, .0f), true);
}

void ACameraPawn::SetSpeedUp(bool bInSpeedUp)
{
	bSpeedUp = bInSpeedUp;
}

