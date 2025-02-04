// Fill out your copyright notice in the Description page of Project Settings.


#include "Movements/SteeringMovementComponent.h"


USteeringMovementComponent::USteeringMovementComponent()
{
	FastPhysicsEngine = CreateDefaultSubobject<UFastPhysicsEngine>(TEXT("FastPhysicsEngine"));
	FastPhysicsEngine->SetAutoActivate(true);
    FastPhysicsEngine->SetPawn(GetOwner<APawn>());
}

void USteeringMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    if (!UpdatedComponent) return;

    UFastPhysicsEngine* fpe = GetOwner()->FindComponentByClass<UFastPhysicsEngine>();

    FVector ForwardVector = UpdatedComponent->GetForwardVector().GetSafeNormal();
    FVector DesiredVelocity = MoveVelocity.GetSafeNormal();

    float ForwardDot = FVector::DotProduct(ForwardVector, DesiredVelocity);
    float RightDot = FVector::CrossProduct(ForwardVector, DesiredVelocity).Z;

    // Calculate movement vector and yawrate
    FVector MoveInput = ForwardVector * ForwardDot * ForwardSpeed * (bForceMaxSpeed ? 1.f : .5f);
    float YawRate = RightDot * RotationSpeed;

    FastPhysicsEngine->SetDesiredVelocity(MoveInput);
    FastPhysicsEngine->SetDesiredYawRate(YawRate);
}
