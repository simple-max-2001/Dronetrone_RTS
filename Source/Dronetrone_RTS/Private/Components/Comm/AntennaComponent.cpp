// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Comm/AntennaComponent.h"

// Sets default values for this component's properties
UAntennaComponent::UAntennaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

float UAntennaComponent::GetAntennaGain(FVector Direction) const
{
	return MaxAntennaGain;
}

