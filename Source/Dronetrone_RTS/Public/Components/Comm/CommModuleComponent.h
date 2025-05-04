// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"

#include "AntennaComponent.h"
#include "RTSTypes.h"

#include "CommModuleComponent.generated.h"

class DRONETRONE_RTS_API UCommModuleComponent;

USTRUCT(BlueprintType)
struct FConnectionInfo
{
	GENERATED_BODY()

	// If module is connected 
	UPROPERTY(BlueprintReadOnly)
	bool bIsOnline = false;

	// Connected relay
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UCommModuleComponent> Relay;

	// Receiver power level, dBm
	UPROPERTY(BlueprintReadOnly)
	float Power = WEAKEST_SIGNAL;

	// Communication frequency, GHz
	UPROPERTY(BlueprintReadOnly)
	float Frequency = 0;
};

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
	// Update connection status of module
	UFUNCTION(BlueprintCallable)
	virtual void UpdateConnection();

	// Find new relay if module is offline 
	UFUNCTION(BlueprintCallable)
	virtual void FindNewRelay();

	// Check if another communication module is available
	virtual bool IsAvailable(const UCommModuleComponent* Other, bool bBidirectional = true, float* Power = nullptr) const;
	
	// Check if another communication module is available
	virtual bool IsAvailable(TWeakObjectPtr<const UCommModuleComponent> Other, bool bBidirectional = true, float* Power = nullptr) const;

	// Check if another communication module is available
	virtual bool IsAvailable(TSoftObjectPtr<const UCommModuleComponent> Other, bool bBidirectional = true, float* Power = nullptr) const;
	
	// Check if another communication module is available as relay
	virtual bool IsAvailableRelay(const UCommModuleComponent* Other = nullptr, float* Power = nullptr) const;

	// Get list of available relays
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual TArray<TSoftObjectPtr<UCommModuleComponent>> SearchRelays(const bool bCheckOwnership) const;
	
	// --- Getters ---

	// Get if communicator is online
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Communication")
	virtual bool IsOnline() const
	{
		return ConnectionInfo.bIsOnline;
	}
	
	// Get if communicator is enabled
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Communication")
	virtual bool IsEnabled() const
	{
		return bIsEnabled;
	}

	// Get if communicator can act as relay
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Communication")
	virtual bool IsRelay() const
	{
		return bIsRelay;
	}
	
	// Get receiver sensitivity, dBm
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Communication")
	virtual float GetReceiverSensitivity() const
	{
		return ReceiverSensitivity;
	}
	
	// Get transmitter power, dBm
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Communication")
	virtual float GetTransmitterPower() const
	{
		return TransmitterPower;
	}
	
	// Get receiver antenna component reference
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Communication")
	TSoftObjectPtr<UAntennaComponent> GetReceiverAntenna() const
	{
		return TSoftObjectPtr<UAntennaComponent>(ReceiverAntenna);
	}
	
	// Get transmitter antenna component reference
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Communication")
	TSoftObjectPtr<UAntennaComponent> GetTransmitterAntenna() const
	{
		return TSoftObjectPtr<UAntennaComponent>(TransmitterAntenna);
	}

	// Get communication frequency, GHz
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Communication")
	virtual float GetFrequency() const
	{
		return CommFrequency;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Communication")
	virtual FConnectionInfo GetConnectionInfo() const
	{
		return FConnectionInfo(ConnectionInfo);
	}

	// Calculate power of received signal, dBm
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Communication")
	float GetSignalPower(const UCommModuleComponent* Other = nullptr, float Frequency = 0, const bool bSelfIsReceiver = true, const bool bCheckCollisions = true) const;
	// --- End getters ---

	// Draw line between antennas
	UFUNCTION(BlueprintCallable, Category = "Communication")
	void DrawConnectionLine(const float LifeTime = .2f) const;

protected:
	// Receiver sensitivity without jamming, dBm
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float ReceiverSensitivity = -42.f;

	// Transmitter power, dBm
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float TransmitterPower = 33.f;

	// Communication frequency, GHz
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float CommFrequency = 2.4f;	

	// Maximum search distance for other communication modules, cm
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	float MaxSearchDistance = 1e4f;

	// Reference to receiver antenna component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	UAntennaComponent* ReceiverAntenna;

	// Reference to transmitter antenna component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Communication")
	UAntennaComponent* TransmitterAntenna;
	
	// Period of update connection, seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Communication")
	float ConnectionUpdatePeriod = .2f;

	// Set if communication module is enabled
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Communication")
	bool bIsEnabled = true;

	// Set if communication module is always online and don't need any relay
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Communication")
	bool bIsSuper = false;
	
	// Set if communication module can operate as relay
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Relay")
	bool bIsRelay = false;

	FConnectionInfo ConnectionInfo;

	// Handler for updating connection
	FTimerHandle ConnectionCheckHandle;
};
