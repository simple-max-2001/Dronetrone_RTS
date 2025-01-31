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
        // Assign owner id
        PlayerState->Setup(0);

	   // TODO: Implement logic for few players
    }
}

ERelationType ARTSGameMode::GetRelation(int32 owner_a, int32 owner_b) const
{
	if (owner_a == owner_b) return ERelationType::OWN;

	int32 owner_s = std::min(owner_a, owner_b);
	int32 owner_g = std::max(owner_a, owner_b);

	if (owner_s == 0 && owner_g == 1)
		return ERelationType::FRIEND;

	if ((owner_s == 0 || owner_s == 1) && owner_g == 2)
		return ERelationType::FOE;

	return ERelationType::NEUTRAL;
}
