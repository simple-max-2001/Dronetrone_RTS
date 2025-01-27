// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "RTSTypes.h"

#include "OwnershipComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API UOwnershipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOwnershipComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category="Ownership")
    void SetFaction(EPlayerFaction NewFaction);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Ownership")
    EPlayerFaction GetFaction() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Ownership")
    bool IsOwnedBy(EPlayerFaction Faction) const;

protected:
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Ownership")
    EPlayerFaction UnitFaction;

};
