// Fill out your copyright notice in the Description page of Project Settings.


#include "Movements/SteeringMovementComponent.h"
#include "Physics/FastPhysicsEngine.h"

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


    if (fpe)
    {
        fpe->SetDesiredVelocity(MoveInput);
        fpe->SetDesiredYawRate(YawRate);
    }
    else
    {
        FRotator RotationDelta = FRotator(0, YawRate * GetWorld()->DeltaTimeSeconds, 0);
        MoveUpdatedComponent(MoveInput * GetWorld()->DeltaTimeSeconds, UpdatedComponent->GetComponentRotation() + FRotator(0, YawRate * GetWorld()->DeltaTimeSeconds, 0), true);
    }
}
