// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelManager.h"
#include "DrawDebugHelpers.h"

// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#if WITH_EDITOR
	DrawAllowedZones();
#endif
}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ALevelManager::isAllowedCameraZone(FVector new_location)
{
	if (bRestrictCameraMovement) return false;

	if (AllowedCameraZones.IsEmpty()) return true;

	for (int32 i = 0; i < AllowedCameraZones.Num(); i++)
	{
		FBox2D zone = AllowedCameraZones[i];

		if (!zone.bIsValid) continue;

		if (new_location.X <= zone.Max.X && new_location.X >= zone.Min.X &&
			new_location.Y <= zone.Max.Y && new_location.Y >= zone.Min.Y)
			return true;
	}

	return false;
}

#if WITH_EDITOR
void ALevelManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (!PropertyChangedEvent.Property)
		return;

	FString name = PropertyChangedEvent.Property->GetNameCPP();

	if (name == TEXT("X") || name == TEXT("Y"))
	{
		for (int32 i = 0; i < AllowedCameraZones.Num(); i++)
		{
			FBox2D zone(AllowedCameraZones[i]);

			if (!zone.bIsValid) continue;

			if (zone.Min.X > zone.Max.X)
			{
				AllowedCameraZones[i].Min.X = zone.Max.X;
				AllowedCameraZones[i].Max.X = zone.Min.X;
			}
		}
	}

	DrawAllowedZones();
}
void ALevelManager::DrawAllowedZones()
{
	// Clear all previousely drawwed lines
	FlushPersistentDebugLines(GetWorld());

	// Draw allowed camera zones
	for (int32 i = 0; i < AllowedCameraZones.Num(); i++)
	{
		FBox2D zone = AllowedCameraZones[i];

		if (!zone.bIsValid) continue;

		// Receive coordinates of zone with Z axis
		FVector MinPoint = FVector(zone.Min, 0.0f);
		FVector MaxPoint = FVector(zone.Max, 1000.0f);

		// Get center and extent of zone
		FVector Center = (MinPoint + MaxPoint) / 2.0f;
		FVector Extent = (MaxPoint - MinPoint) / 2.0f;

		// Draw zone
		DrawDebugBox(GetWorld(), Center, Extent, FColor::Green, true, -1.0f, 0, 15.0f);
	}
}
#endif


