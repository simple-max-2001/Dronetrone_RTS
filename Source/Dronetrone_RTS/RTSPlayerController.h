// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "RTSPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARTSPlayerController();

	/* MappingContext */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* MappingContext;

	/* Camera Move Input Action */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveCameraAction;

	/* Camera Zoom Input Action */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ZoomCameraAction;

	/* Camera Pan Input Action */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PanCameraAction;

	/* Speed Up Camera Input Action */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpeedUpAction;

	/* Camera Pan Input Action */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float EdgeScrollBorder = .05f;

	///** Jump Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	//class UInputAction* SetDestinationTouchAction;

protected:
	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	void EdgeScroll();

	/** Input handlers for SetDestination action. */
	void OnMoveCamera(const FInputActionInstance& Instance);
	void OnZoomCamera(const FInputActionInstance& Instance);
	void OnPanCamera();
	void OnSpeedUp(bool speed_up);

	void SetDisableCameraMovement(bool bInDisableCameraMovement);
	void SetDisableEdgeScroll(bool bInDisableEdgeScroll);

private:
	bool bDisableCameraMovement = false;
	bool bDisableEdgeScroll = false;

};


