// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UENUM(BlueprintType)
// Enumerator for armor type
enum class EArmorType : uint8 {
	LIGHT = 0  UMETA(DisplayName = "Light"),
	MEDIUM = 1 UMETA(DisplayName = "Medium"),
	HEAVY = 2  UMETA(DisplayName = "Heavy")
};

USTRUCT(BlueprintType)
// Stucture for dealing damage to objects
struct FDamageData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageToLightArmor = .0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageToMediumArmor = .0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageToHeavyArmor = .0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthZero);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealthPercentage();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(FDamageData damage);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnHealthZero OnHealthZero;

protected:
	UPROPERTY(BlueprintReadWrite, Category = Health)
	float Health = 100;

	UPROPERTY(BlueprintReadWrite, Category = Health)
	float MaxHealth = 100;

	UPROPERTY(BlueprintReadWrite, Category = Health)
	EArmorType Armor = EArmorType::LIGHT;

	UPROPERTY(BlueprintReadWrite, Category = Health)
	bool bIsImmortar = false;
		
};
