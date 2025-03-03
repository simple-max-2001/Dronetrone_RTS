// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Comm/CommModuleComponent.h"

#include "CommRelayComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API UCommRelayComponent : public UCommModuleComponent
{
	GENERATED_BODY()

public:
	void StartChecking();

	void StopChecking();

	
};
