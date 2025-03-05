// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AntennaComponent.h"

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

	// Get receiver sensitivity, dBm
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetReceiverSensitivity() const;

	// Get transmitter power, dBm
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetTransmitterPower() const;

	// Get receiver antenna component reference
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSoftObjectPtr<UAntennaComponent> GetReceiverAntenna() const;

	// Get transmitter antenna component reference
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSoftObjectPtr<UAntennaComponent> GetTransmitterAntenna() const;

	// Get communication frequency, GHz
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetFrequency() const;

	// Calculate power of received signal, dBm
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSignalPower(const UCommModuleComponent* Other = nullptr, float Frequency = 0, const bool bSelfIsReceiver = true, const bool bCheckCollisions = true) const;

protected:
	bool bIsConnected = false;

	// Receiver sensitivity without jamming, dBm
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float ReceiverSensitivity = -42.f;

	// Transmitter power, dBm
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float TransmitterPower = 33.f;

	// Receiver antenna gain, dBi
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float ReceiverGain = 0.f;

	// Transmitter antenna gain, dBi
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float TransmitterGain = 0.f;

	// Communication frequency, GHz
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float CommFrequency = 2.4f;	

	// Maximum search distance for other communication modules, cm
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float MaxSearchDistance = 1e4f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Communication")
	TWeakObjectPtr<UCommRelayComponent> CurrentRelay;

	// Reference to receiver antenna component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	UAntennaComponent* ReceiverAntenna;

	// Reference to transmitter antenna component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	UAntennaComponent* TransmitterAntenna;

};
