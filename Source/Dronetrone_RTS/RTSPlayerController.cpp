// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "CameraPawn.h"
#include "RTSHUD.h"
#include "RTSPlayerState.h"

ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	bShouldPerformFullTickWhenPaused = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void ARTSPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(this))
	{

		//Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (MappingContext)
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("MappingContext is null!"));
			}
		}
	}

    if (IsLocalController()) // Make selection manager only for local player
    {
        SelectionManager = GetWorld()->SpawnActor<ASelectionManager>();

		ARTSPlayerState* ps = GetPlayerState<ARTSPlayerState>();
		if (SelectionManager && ps)
		{
			SelectionManager->SetOwnerID(ps->GetOwnerID());
			if (ARTSHUD* hud = GetHUD<ARTSHUD>()) hud->SetSelectionManager(SelectionManager);
		}
    }
}

void ARTSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EdgeScroll();
}

void ARTSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Log, TEXT("SetupInputComponent"));

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if (MoveCameraAction)
		{
			EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &ARTSPlayerController::OnMoveCamera);
			EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Started, this, &ARTSPlayerController::SetDisableEdgeScroll, true);
			EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Completed, this, &ARTSPlayerController::SetDisableEdgeScroll, false);
			EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Canceled, this, &ARTSPlayerController::SetDisableEdgeScroll, false);
		}
		else UE_LOG(LogTemp, Warning, TEXT("MoveCameraAction is null!"));

		if (ZoomCameraAction) EnhancedInputComponent->BindAction(ZoomCameraAction, ETriggerEvent::Triggered, this, &ARTSPlayerController::OnZoomCamera);
		else UE_LOG(LogTemp, Warning, TEXT("ZoomCameraAction is null!"));

		if (PanCameraAction)
		{
			EnhancedInputComponent->BindAction(PanCameraAction, ETriggerEvent::Triggered, this, &ARTSPlayerController::OnPanCamera);
			EnhancedInputComponent->BindAction(PanCameraAction, ETriggerEvent::Started, this, &ARTSPlayerController::SetDisableEdgeScroll, true);
			EnhancedInputComponent->BindAction(PanCameraAction, ETriggerEvent::Completed, this, &ARTSPlayerController::SetDisableEdgeScroll, false);
			EnhancedInputComponent->BindAction(PanCameraAction, ETriggerEvent::Canceled, this, &ARTSPlayerController::SetDisableEdgeScroll, false);
		}
		else UE_LOG(LogTemp, Warning, TEXT("PanCameraAction is null!"));

		if (SpeedUpAction)
		{
			EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Started, this, &ARTSPlayerController::OnSpeedUpCamera, true);
			EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Completed, this, &ARTSPlayerController::OnSpeedUpCamera, false);
			EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Canceled, this, &ARTSPlayerController::OnSpeedUpCamera, false);
		}
		else UE_LOG(LogTemp, Warning, TEXT("SpeedUpAction is null!"));

		if (PauseAction)
		{
			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ARTSPlayerController::OnSwitchPause);
		}
		else UE_LOG(LogTemp, Warning, TEXT("PauseAction is null!"));

		if (SelectAction)
		{
			EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ARTSPlayerController::OnSelectTrigger);
			EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &ARTSPlayerController::OnSelectStart);
			EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &ARTSPlayerController::OnSelectStop);
			EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Canceled, this, &ARTSPlayerController::OnSelectStop);
		}
		else UE_LOG(LogTemp, Warning, TEXT("SelectAction is null!"));

		if (SetDestinationAction)
		{
			EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Started, this, &ARTSPlayerController::OnSetDestination);
		}
		else UE_LOG(LogTemp, Warning, TEXT("SetDestinationAction is null!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent not checked"));
	}
}

void ARTSPlayerController::OnMoveCamera(const FInputActionInstance& Instance)
{
	if (bDisableCameraMovement) return;

	//UE_LOG(LogTemp, Log, TEXT("OnMoveCamera"));
	FVector2D axes = Instance.GetValue().Get<FVector2D>();
	ACameraPawn* pawn = Cast<ACameraPawn>(GetPawn());
	pawn->MovePawn(axes.X, axes.Y);
}

void ARTSPlayerController::OnZoomCamera(const FInputActionInstance& Instance)
{
	//UE_LOG(LogTemp, Log, TEXT("OnZoomCamera"));
	float axis = Instance.GetValue().Get<float>();
	ACameraPawn* pawn = Cast<ACameraPawn>(GetPawn());
	pawn->ChangeZoom(axis);
}

void ARTSPlayerController::OnPanCamera()
{
	float axis_x, axis_y;
	int32 size_x, size_y;

	GetInputMouseDelta(axis_x, axis_y);
	GetViewportSize(size_x, size_y);

	axis_x /= size_x;
	axis_y /= size_y;

	//UE_LOG(LogTemp, Log, TEXT("OnPanCamera: %.5f, %.5f"), axis_x, axis_y);
	ACameraPawn* pawn = Cast<ACameraPawn>(GetPawn());
	pawn->PanCamera(axis_x, axis_y);
}

void ARTSPlayerController::OnSpeedUpCamera(bool speed_up)
{
	ACameraPawn* pawn = Cast<ACameraPawn>(GetPawn());
	pawn->SetSpeedUp(speed_up);
}

void ARTSPlayerController::OnSwitchPause()
{
	ARTSGameState* game_state = GetWorld()->GetGameState<ARTSGameState>();

	if (game_state)
		game_state->ServerSetPause(!game_state->bIsGamePaused);
}

void ARTSPlayerController::OnSelectStart()
{
	ARTSPlayerState* ps = GetPlayerState<ARTSPlayerState>();

	if (!ps || !SelectionManager) return;

	UE_LOG(LogTemp, Log, TEXT("OnSelectStart"));

	// Start HUD selection
	if (ARTSHUD* hud = GetHUD<ARTSHUD>()) hud->StartSelection();

    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

    if (HitResult.bBlockingHit)
    {
		ABaseUnit* unit = Cast<ABaseUnit>(HitResult.GetActor());

		SelectionManager->SelectUnit(unit);
	}
}

void ARTSPlayerController::OnSelectTrigger()
{
	// Stop HUD selection
	//if (ARTSHUD* hud = GetHUD<ARTSHUD>()) hud->StartSelection();
}

void ARTSPlayerController::OnSelectStop()
{
	// Stop HUD selection
	if (ARTSHUD* hud = GetHUD<ARTSHUD>()) hud->EndSelection();
}

void ARTSPlayerController::OnSetDestination()
{
	ARTSPlayerState* ps = GetPlayerState<ARTSPlayerState>();

	if (!ps || !SelectionManager) return;

	UE_LOG(LogTemp, Log, TEXT("OnSetDestination"));
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitResult);

    if (HitResult.bBlockingHit)
    {
        FVector TargetLocation = HitResult.ImpactPoint;
        DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 12, FColor::Green, false, 5.0f);

		for (TSoftObjectPtr<ABaseUnit> unit : SelectionManager->GetSelectedUnits())
		{
			unit->ControlComponent->MoveToLocation(TargetLocation);
		}
    }
}

void ARTSPlayerController::EdgeScroll()
{
	if (bDisableEdgeScroll) return;

	float axis_x = .0f, axis_y = .0f;

	float mouse_x, mouse_y;
	int32 size_x, size_y;

	GetMousePosition(mouse_x, mouse_y);
	GetViewportSize(size_x, size_y);

	mouse_x /= size_x;
	mouse_y /= size_y;

	if (mouse_x < EdgeScrollBorder) axis_y = -1.f;
	else if (mouse_x > 1 - EdgeScrollBorder) axis_y = 1.f;

	if (mouse_y < EdgeScrollBorder) axis_x = 1.f;
	else if (mouse_y > 1 - EdgeScrollBorder) axis_x = -1.f;

	if (axis_x != .0f || axis_y != .0f)
	{
		SetDisableCameraMovement(true);

		ACameraPawn* pawn = Cast<ACameraPawn>(GetPawn());
		pawn->MovePawn(axis_x, axis_y);
	}
	else
	{
		SetDisableCameraMovement(false);
	}
}

void ARTSPlayerController::SetDisableCameraMovement(bool bInDisableCameraMovement)
{
	bDisableCameraMovement = bInDisableCameraMovement;
}

void ARTSPlayerController::SetDisableEdgeScroll(bool bInDisableEdgeScroll)
{
	bDisableEdgeScroll = bInDisableEdgeScroll;
}
