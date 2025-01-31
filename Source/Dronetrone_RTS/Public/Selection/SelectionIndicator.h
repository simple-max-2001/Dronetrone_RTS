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

    /** Прив’язка до юніта */
	UFUNCTION(BlueprintCallable)
    void AttachToEntity(AActor* entity, float lifetime = .0f);

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
