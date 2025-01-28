// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"

#include "RTSTypes.h"

#include "SelectionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectChanged, bool, isSelected);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API USelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USelectionComponent();

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

    UFUNCTION(BlueprintCallable, Category="Selection")
    void SetDecalComponent(UDecalComponent* decalComponent);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Selection")
    bool IsSelected() const;

    UFUNCTION(BlueprintCallable, Category="Selection")
    void Select(EPlayerFaction player_faction);

    UFUNCTION(BlueprintCallable, Category="Selection")
    void Unselect(EPlayerFaction player_faction);

protected:
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Ownership")
    EPlayerFaction UnitFaction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
    UMaterialInterface* OwnSelectionMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
    UMaterialInterface* FriendSelectionMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
    UMaterialInterface* NeutralSelectionMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
    UMaterialInterface* FoeSelectionMaterial;

    UPROPERTY(BlueprintAssignable)
    FOnSelectChanged OnSelectChanged;

    bool bIsSelected = false;

    UDecalComponent* DecalComponent;

};
