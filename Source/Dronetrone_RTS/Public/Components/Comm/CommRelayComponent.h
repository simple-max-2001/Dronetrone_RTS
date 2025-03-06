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

	UCommRelayComponent();

protected:
	virtual void BeginPlay() override;
	
public:	
	void ResetChecked();

	void MarkAsChecked();

	bool TryToConnectWith(const UCommRelayComponent* Relay, bool bOwnOnly);
	
	virtual void UpdateConnection() override;

	bool GetIsSuper() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Relay")
	bool bIsSuper = false;

	bool bIsChecked = false;
};
