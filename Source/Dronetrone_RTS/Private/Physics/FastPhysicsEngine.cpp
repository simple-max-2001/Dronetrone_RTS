// Fill out your copyright notice in the Description page of Project Settings.

#include "Physics/FastPhysicsEngine.h"
#include "AIController.h"
#include "GameFramework/NavMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values for this component's properties
UFastPhysicsEngine::UFastPhysicsEngine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UFastPhysicsEngine::SetPawn(APawn* pawn)
{
    Pawn = pawn;
}

// Called when the game starts
void UFastPhysicsEngine::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UFastPhysicsEngine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Pawn) return;

    ApplyMovement(DeltaTime);
	CheckCollisions(DeltaTime);
}

void UFastPhysicsEngine::SetDesiredVelocity(FVector Velocity)
{
    DesiredVelocity_ = Velocity;
}

void UFastPhysicsEngine::SetDesiredYawRate(float YawRate)
{
    DesiredYawRate_ = YawRate;
}

void UFastPhysicsEngine::ApplyMovement(float DeltaTime)
{
    FVector DesiredVelocity = DesiredVelocity_;

    if (bUseGravity) DesiredVelocity.Z = 0;

    if (bDrawDebug)
    {
        // Draw forward vector
        FVector ForwardVector = Pawn->GetActorForwardVector();
        DrawDebugLine(GetWorld(), Pawn->GetActorLocation(), Pawn->GetActorLocation() + ForwardVector * 150, FColor(0, 0, 255));

        // Draw desired velocity vector
        FVector DesiredVelocityNormal = DesiredVelocity.GetSafeNormal();
        DrawDebugLine(GetWorld(), Pawn->GetActorLocation(), Pawn->GetActorLocation() + DesiredVelocityNormal * 150, FColor(255, 255, 0));
    }

    CurrentVelocity_ = FMath::VInterpTo(FVector(CurrentVelocity_), DesiredVelocity, DeltaTime, InterpVelocity);
    //UE_LOG(LogTemp, Log, TEXT("Current velocity: %s (%.1f cm/s)"), *CurrentVelocity_.ToString(), CurrentVelocity_.Length());

    // Apply movement and rotation;
    Pawn->AddActorWorldOffset(CurrentVelocity_ * DeltaTime, true);
    Pawn->AddActorLocalRotation(FRotator(.0f, DesiredYawRate_*DeltaTime, .0f));

    DesiredVelocity_ = FVector::ZeroVector;
    DesiredYawRate_ = .0f;
}

void UFastPhysicsEngine::CheckCollisions(float DeltaTime)
{
    bIsGrounded = false;
    
    if (!bUseGravity) return;

    FVector ActorLocation = Pawn->GetActorLocation();
    FRotator ActorAttitude = Pawn->GetActorRotation();

    FVector VForwardOffset = ActorAttitude.RotateVector(FVector(ForwardOffset, 0.0f, 0.0f));
    FVector VRightOffset = ActorAttitude.RotateVector(FVector(0.0f, RightOffset, 0.0f));

    // Make array of points
    FVector Points[4] = {
        ActorLocation + VForwardOffset + VRightOffset,  // Forward-Right
        ActorLocation + VForwardOffset - VRightOffset,  // Forward-Left
        ActorLocation - VForwardOffset + VRightOffset,  // Backward-Right
        ActorLocation - VForwardOffset - VRightOffset   // Backward-Left
    };

    // Ignore own collisions
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(Pawn);

    float NewAltitude = ActorLocation.Z;
    
    float DeltaAltitude = (FallSpeed + .5f * FallAcceleration * DeltaTime) * DeltaTime;
    NewAltitude -= DeltaAltitude;
    FallSpeed += FallAcceleration * DeltaTime;

    for (int i = 0; i < 4; i++)
    {
        FVector Start = Points[i] + FVector(0, 0, RequiredClearance);
        FVector End = Points[i] - FVector(0, 0, DeltaAltitude + RequiredClearance); // Calculate end point with change altitude and required clearance

        FHitResult HitResult;

        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, CollisionParams))
        {
            float HitAltitude = HitResult.Location.Z + RequiredClearance;
            if (NewAltitude < HitAltitude)
            {
                NewAltitude = HitAltitude;
                FallSpeed = 0;
                bIsGrounded = true;
            }
        }

        // Draw line trace lines for debugging
        if (bDrawDebug) DrawDebugLine(GetWorld(), Start + FVector(0, 0, 50), End, FColor(0, 255u, 0));
    }

    ActorLocation.Z = NewAltitude;
    Pawn->SetActorLocation(ActorLocation);
}

