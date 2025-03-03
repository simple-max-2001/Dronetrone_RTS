// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CommModuleComponent.generated.h"

class UCommRelayComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONETRONE_RTS_API UCommModuleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCommModuleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable)
	bool IsConnected() const;
	
	UFUNCTION(BlueprintCallable)
	virtual void UpdateConnection();

	UFUNCTION(BlueprintCallable)
	virtual bool FindNewRelay();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool CheckRelay(UCommRelayComponent* Other = nullptr) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool CanCommunicateWithModule(const UCommModuleComponent* Other, bool bBidirectional = true, bool bCheckOwnership = true) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual TArray<TSoftObjectPtr<UCommModuleComponent>> GetAvailableComms(const bool bCheckOwnership) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual TArray<TSoftObjectPtr<UCommRelayComponent>> GetAvailableRelays(const bool bCheckOwnership) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual double GetReceiverSensitivity() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual double GetTransmitterPower() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual double GetReceiverGain(FVector Direction) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual double GetTransmitterGain(FVector Direction) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual double GetFrequency() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static double GetSignalPower(const UCommModuleComponent* Transmitter, const UCommModuleComponent* Receiver, const FVector Distance, const float Frequency);

protected:
	bool bIsConnected = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CommModuleComponent")
	double ReceiverSensitivity = 0.f;	// Receiver sensitivity without jamming, W
	double TransmitterPower = 1.f;		// Transmitter power, W
	
	double ReceiverGain = 1.f;		// Receiver antenna gain
	double TransmitterGain = 1.f;	// Transmitter antenna gain

	double Frequency = 1e10f;	// Let's assume that is "talking" on 10 GHz

	// Maximum search distance for other communication modules
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxSearchDistance = 1e4f;
	
	TWeakObjectPtr<UCommRelayComponent> CurrentRelay;

};
