// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSGameMode.h"

#include "CameraPawn.h"
#include "RTSGameState.h"
#include "RTSHUD.h"
#include "RTSPlayerController.h"
#include "RTSPlayerState.h"

ARTSGameMode::ARTSGameMode()
{
	// use our custom GameState class
	GameStateClass = ARTSGameState::StaticClass();

	HUDClass = ARTSHUD::StaticClass();

	// use our custom PlayerController class
	PlayerControllerClass = ARTSPlayerController::StaticClass();

	// use our custom PlayerState class
	PlayerStateClass = ARTSPlayerState::StaticClass();

	// set default pawn class to our Blueprinted character
	DefaultPawnClass = ACameraPawn::StaticClass();

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<ARTSPlayerController> PlayerControllerBPClass(TEXT("/Game/RTS/Player/BP_PlayerController.BP_PlayerController_C"));

	if (PlayerControllerBPClass.Class)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerControllerBPClass is null!"));
	}

}

void ARTSGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ARTSGameMode::PreLogin(const FString& Options, 
                            const FString& Address, 
                            const FUniqueNetIdRepl& UniqueId, 
                            FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

    if (GetNumPlayers() >= 1) 
    {
        ErrorMessage = TEXT("Server is fulfilled!");
    }
}

void ARTSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // Receive PlayerState of player
    if (ARTSPlayerState* PlayerState = NewPlayer->GetPlayerState<ARTSPlayerState>())
    {
        // Assign owner id
        PlayerState->Setup(0);

	   // TODO: Implement logic for few players
    }
}

ERelationType ARTSGameMode::GetRelation(int32 Owner_A, int32 Owner_B) const
{
	if (Owner_A == Owner_B) return ERelationType::Own;

	int32 Owner_S = std::min(Owner_A, Owner_B);
	int32 Owner_G = std::max(Owner_A, Owner_B);

	if (Owner_S == 0 && Owner_G == 1)
		return ERelationType::Friend;

	if ((Owner_S == 0 || Owner_S == 1) && Owner_G == 2)
		return ERelationType::Foe;

	return ERelationType::Neutral;
}
