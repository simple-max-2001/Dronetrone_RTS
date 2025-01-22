// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGameState.h"
#include "Kismet/GameplayStatics.h"

void ARTSGameState::ServerSetPause_Implementation(bool bPause)
{
    bIsGamePaused = bPause;
    OnRep_IsGamePaused();
}

bool ARTSGameState::ServerSetPause_Validate(bool bPause)
{
    return true;
}

void ARTSGameState::OnRep_IsGamePaused()
{
    if (bIsGamePaused)
    {
        UGameplayStatics::SetGamePaused(GetWorld(), true);
    }
    else
    {
        UGameplayStatics::SetGamePaused(GetWorld(), false);
    }
}
