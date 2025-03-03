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

	// Get receiver sensitivity, dB
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetReceiverSensitivity() const;

	// Get transmitter power, dB
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetTransmitterPower() const;

	// Get receiver antenna gain, dB
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetReceiverGain(FVector Direction) const;

	// Get transmitter antenna gain, dB
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetTransmitterGain(FVector Direction) const;

	// Get communication frequency, GHz
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetFrequency() const;

	// Calculate power of received signal, dB
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float GetSignalPower(const UCommModuleComponent* Transmitter, const UCommModuleComponent* Receiver, const FVector Distance, const float Frequency);

protected:
	bool bIsConnected = false;

	// Receiver sensitivity without jamming, dB
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float ReceiverSensitivity = -75.f;

	// Transmitter power, dB
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float TransmitterPower = 3.f;

	// Receiver antenna gain, dB
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float ReceiverGain = 0.f;

	// Transmitter antenna gain, dB
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float TransmitterGain = 0.f;

	// Communication frequency, GHz
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float Frequency = 2.4f;	

	// Maximum search distance for other communication modules, cm
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float MaxSearchDistance = 1e4f;
	
	TWeakObjectPtr<UCommRelayComponent> CurrentRelay;

};
