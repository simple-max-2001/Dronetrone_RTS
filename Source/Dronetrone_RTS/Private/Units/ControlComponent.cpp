// Fill out your copyright notice in the Description page of Project Settings.

#include "Units/ControlComponent.h"
#include "AIController.h"

// Sets default values for this component's properties
UControlComponent::UControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UControlComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UControlComponent::MoveToLocation(const FVector& Destination, float AcceptanceRadius)
{
    if (!bIsControllable) return;

    CurrentDestination = Destination;
    AAIController* AIController = Cast<AAIController>(GetOwner<APawn>()->GetController());
    if (AIController)
    {
        AIController->MoveToLocation(Destination, AcceptanceRadius);
    }
}

void UControlComponent::StopMovement()
{
    if (!bIsControllable) return;

    AAIController* AIController = Cast<AAIController>(GetOwner<APawn>()->GetController());
    if (AIController)
    {
        AIController->StopMovement();
    }
}

void UControlComponent::RotateToFace(FVector Target)
{
    if (!bIsControllable) return;

    FRotator NewRotation = (Target - GetOwner()->GetActorLocation()).Rotation();
    GetOwner()->SetActorRotation(NewRotation);
}

void UControlComponent::AssignNewTarget(AActor* NewTarget, float AcceptanceRadius)
{
    if (!bIsControllable) return;

    TargetActor = NewTarget;
    if (TargetActor)
    {
        MoveToLocation(TargetActor->GetActorLocation(), AcceptanceRadius);
    }
}

void UControlComponent::SetUncontrollable()
{
    StopMovement();
    bIsControllable = false;
}