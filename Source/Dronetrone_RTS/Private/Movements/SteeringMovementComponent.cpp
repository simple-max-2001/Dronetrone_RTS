// Fill out your copyright notice in the Description page of Project Settings.


#include "Movements/SteeringMovementComponent.h"

void USteeringMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    if (!UpdatedComponent)
    {
        return;
    }

    FVector ForwardVector = UpdatedComponent->GetForwardVector().GetSafeNormal();
    FVector DesiredVelocity = MoveVelocity.GetSafeNormal();

    // Обчислення повороту (перехід до потрібного напрямку)
    float ForwardDot = FVector::DotProduct(ForwardVector, DesiredVelocity);
    float RightDot = FVector::CrossProduct(ForwardVector, DesiredVelocity).Z;

    // Рух вперед або назад
    FVector MoveInput = ForwardVector * ForwardDot * ForwardSpeed * (bForceMaxSpeed ? 1.f : .5f);

    // Обчислення кута повороту
    FRotator RotationDelta = FRotator(0, RightDot * RotationSpeed * GetWorld()->DeltaTimeSeconds, 0);

    // Виконуємо переміщення та поворот
    MoveUpdatedComponent(MoveInput * GetWorld()->DeltaTimeSeconds, UpdatedComponent->GetComponentRotation() + RotationDelta, true);

}
