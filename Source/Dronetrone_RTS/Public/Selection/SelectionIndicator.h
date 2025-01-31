// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "SelectionIndicator.generated.h"

UCLASS()
class DRONETRONE_RTS_API ASelectionIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectionIndicator();

    // Attach indicator to unit
	UFUNCTION(BlueprintCallable)
    bool AttachToEntity(TSoftObjectPtr<AActor> entity, int32 owner_id, float lifetime = .0f);

protected:
    UDecalComponent* DecalComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
    UMaterialInterface* OwnSelectionMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
    UMaterialInterface* FriendSelectionMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
    UMaterialInterface* NeutralSelectionMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
    UMaterialInterface* FoeSelectionMaterial;
};
