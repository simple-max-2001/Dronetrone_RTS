// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	//UEntityComponent* entity = Cast<UEntityComponent>(UGameplayStatics::GetActorOfClass(GetOwner(), UEntityComponent::StaticClass()));
	
	//if (entity)	OnHealthZero.AddDynamic(this, &entity->KillEntity);
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHealthComponent::GetHealth()
{
	return Health;
}

float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}

float UHealthComponent::GetHealthPercentage()
{
	return Health / MaxHealth;
}

void UHealthComponent::TakeDamage(FDamageData damage)
{
	// If entity is immortar, do nothing
	if (bIsImmortar) return;

	// If entity is already dead, do nothing
	if (Health <= 0) return;

	float damage_ = .0f;

	// Choose damage depending on armor type
	switch (Armor)
	{
	case EArmorType::LIGHT:
		damage_ = damage.DamageToLightArmor;
		break;

	case EArmorType::MEDIUM:
		damage_ = damage.DamageToMediumArmor;
		break;

	case EArmorType::HEAVY:
		damage_ = damage.DamageToHeavyArmor;
		break;

	default:
		return;
	}

	// If there are no damage, do nothing
	if (damage_ <= 0) return;

	Health -= damage_;

	if (Health <= 0)
	{
		// If lost health, do OnHealthZero broadcast
		Health = 0;
		if (OnHealthZero.IsBound()) OnHealthZero.Broadcast();
	}
	else
	{
		// Else do OnHealthChanged broadcast
		if (OnHealthChanged.IsBound()) OnHealthChanged.Broadcast();
	}
}

