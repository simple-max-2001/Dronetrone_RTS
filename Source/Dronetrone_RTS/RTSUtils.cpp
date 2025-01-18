// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "Landscape.h"
#include "RTSUtils.h"

ALevelManager* URTSUtils::GetLevelManager(const UWorld* world)
{
    return Cast<ALevelManager>(UGameplayStatics::GetActorOfClass(world, ALevelManager::StaticClass()));
}
