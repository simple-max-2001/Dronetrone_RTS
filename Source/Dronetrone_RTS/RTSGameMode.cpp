// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSGameMode.h"

#include "CameraPawn.h"
#include "RTSGameState.h"
#include "RTSPlayerController.h"
#include "RTSPlayerState.h"

ARTSGameMode::ARTSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARTSPlayerController::StaticClass();

	PlayerStateClass = ARTSPlayerState::StaticClass();

	// set default pawn class to our Blueprinted character
	DefaultPawnClass = ACameraPawn::StaticClass();

	// set default controller to our Blueprinted controller /Script/Engine.Blueprint'/Game/RTS/Player/BP_PlayerController.BP_PlayerController'
	static ConstructorHelpers::FClassFinder<ARTSPlayerController> PlayerControllerBPClass(TEXT("/Game/RTS/Player/BP_PlayerController.BP_PlayerController_C"));

	if (PlayerControllerBPClass.Class)
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

	// ARTSGameState* gs = GetGameState<ARTSGameState>();

	// if (gs)
	// {
	// 	gs->AddPlayerInfo(FPlayerInfo(EPlayerFaction::PLAYER_1));
	// 	gs->AddPlayerInfo(FPlayerInfo(EPlayerFaction::PLAYER_2, false));
	// }
	// ALandscape* Landscape = Cast<ALandscape>(UGameplayStatics::GetActorOfClass(GetWorld(), ALandscape::StaticClass()));
	/*if (Landscape)
	{
		ULevelManager* level_mgr = Landscape->FindComponentByClass<ULevelManager>();

		if (level_mgr) LevelManager = level_mgr;
	}*/
}

void ARTSGameMode::PreLogin(const FString& Options, 
                            const FString& Address, 
                            const FUniqueNetIdRepl& UniqueId, 
                            FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

    const int32 MaxPlayers = 1;
    if (GetNumPlayers() >= MaxPlayers) 
    {
        ErrorMessage = TEXT("Server is fullfilled!");
    }
}

void ARTSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // Receive PlayerState of player
    ARTSPlayerState* PlayerState = NewPlayer->GetPlayerState<ARTSPlayerState>();
    if (PlayerState)
    {
        // Assign player faction унікальний TeamID
        PlayerState->Setup(EPlayerFaction::PLAYER_1);

	   // TODO: Implement logic for few players
    }
}


ERelationType ARTSGameMode::GetRelation(EPlayerFaction own_faction, EPlayerFaction players_faction) const
{
	if (own_faction == players_faction) return ERelationType::OWN;

	if ((own_faction == EPlayerFaction::PLAYER_1 && players_faction == EPlayerFaction::PLAYER_2) ||
		(own_faction == EPlayerFaction::PLAYER_2 && players_faction == EPlayerFaction::PLAYER_1))
		return ERelationType::FOE;

	return ERelationType::NEUTRAL;
}
