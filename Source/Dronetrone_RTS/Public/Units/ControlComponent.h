// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ControlComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API UControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

    UFUNCTION(BlueprintCallable, Category="Control")
    void MoveToLocation(const FVector& Destination, float AcceptanceRadius = 20.f);

    UFUNCTION(BlueprintCallable, Category="Control")
    void StopMovement();

    UFUNCTION(BlueprintCallable, Category="Control")
    void RotateToFace(FVector Target);

    UFUNCTION(BlueprintCallable, Category="Control")
    void AssignNewTarget(AActor* NewTarget, float AcceptanceRadius = 120.f);

    UFUNCTION(BlueprintCallable, Category="Control")
    void SetUncontrollable();

private:
    UPROPERTY()
    FVector CurrentDestination;

    UPROPERTY()
    AActor* TargetActor;

    bool bIsControllable = true;
};
