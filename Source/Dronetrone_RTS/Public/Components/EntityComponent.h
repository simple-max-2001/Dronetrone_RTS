// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"

#include "RTSTypes.h"

#include "EntityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnEntity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDieEntity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroyEntity);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API UEntityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEntityComponent();

	UFUNCTION(BlueprintCallable)
	void KillEntity();

	UFUNCTION(BlueprintCallable)
	void DestroyEntity();

	UPROPERTY(BlueprintAssignable)
	FSpawnEntity OnSpawnEntity;

	UPROPERTY(BlueprintAssignable)
	FDieEntity OnDieEntity;

	UPROPERTY(BlueprintAssignable)
	FDieEntity OnDestroyEntity;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Entity")
	bool IsAlive() const
	{
		return bIsAlive;
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ownership")
	bool IsOwnedBy(int32 Owner_ID) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ownership")
	bool IsFriend(UEntityComponent* OtherEntity, bool bOwnerOnly = true) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ownership")
	ERelationType GetRelation(int32 Owner_ID) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	float GetSelectionRadius() const
	{
		return SelectionRadius;
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ownership")
	int32 GetOwnerID() const
	{
		return OwnerID;
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Entity")
	FGuid GetEntityID() const
	{
		return EntityID;
	}

	bool operator==(const UEntityComponent& Other) const
	{
		return EntityID == Other.EntityID;
	}
	
protected:
	bool bIsAlive = true;

	UPROPERTY(EditInstanceOnly, Category = "Ownership")
	int32 OwnerID = -1;

	UPROPERTY(EditInstanceOnly, Category = "Ownership")
	FGuid EntityID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Selection")
	float SelectionRadius = 128;

};
