// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSGameMode.h"
#include "RTSPlayerController.h"
#include "RTSGameState.h"
#include "CameraPawn.h"
#include "UObject/ConstructorHelpers.h"

ARTSGameMode::ARTSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARTSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	DefaultPawnClass = ACameraPawn::StaticClass();

	// set default controller to our Blueprinted controller /Script/Engine.Blueprint'/Game/RTS/Player/BP_PlayerController.BP_PlayerController'
	static ConstructorHelpers::FClassFinder<ARTSPlayerController> PlayerControllerBPClass(TEXT("/Game/RTS/Player/BP_PlayerController.BP_PlayerController_C"));

	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerControllerBPClass is null!"));
	}

	GameStateClass = ARTSGameState::StaticClass();
}

void ARTSGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ALandscape* Landscape = Cast<ALandscape>(UGameplayStatics::GetActorOfClass(GetWorld(), ALandscape::StaticClass()));
	/*if (Landscape)
	{
		ULevelManager* level_mgr = Landscape->FindComponentByClass<ULevelManager>();

		if (level_mgr) LevelManager = level_mgr;
	}*/
}

void ARTSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ARTSGameState* GameState_ = GetGameState<ARTSGameState>();
	if (GameState_)
	{
		int32 PlayerIndex = GetNumPlayers() - 1; // Отримати індекс нового гравця
		FPlayerInfo* PlayerInfo = GameState_->GetPlayerInfo();

		if (PlayerInfo)
		{
			ARTSPlayerController* PC = Cast<ARTSPlayerController>(NewPlayer);
			if (PC)
			{
				PC->SetPlayerInfo(PlayerInfo);
			}
		}
	}
}


