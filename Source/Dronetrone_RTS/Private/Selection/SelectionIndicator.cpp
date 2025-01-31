// Fill out your copyright notice in the Description page of Project Settings.


#include "Selection/SelectionIndicator.h"

// Sets default values
ASelectionIndicator::ASelectionIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> OwnSelectionMaterialFinder(    TEXT("/Game/Materials/Selection/M_SelectionOwn.M_SelectionOwn"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> FriendSelectionMaterialFinder( TEXT("/Game/Materials/Selection/M_SelectionFriend.M_SelectionFriend"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> NeutralSelectionMaterialFinder(TEXT("/Game/Materials/Selection/M_SelectionNeutral.M_SelectionNeutral"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> FoeSelectionMaterialFinder(    TEXT("/Game/Materials/Selection/M_SelectionFoe.M_SelectionFoe"));

    if (OwnSelectionMaterialFinder.Succeeded())
        OwnSelectionMaterial = OwnSelectionMaterialFinder.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("OwnSelectionMaterial is not setted!!!"));
    if (FriendSelectionMaterialFinder.Succeeded())
        FriendSelectionMaterial = FriendSelectionMaterialFinder.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("FriendSelectionMaterial is not setted!!!"));
    if (NeutralSelectionMaterialFinder.Succeeded())
        NeutralSelectionMaterial = NeutralSelectionMaterialFinder.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("NeutralSelectionMaterial is not setted!!!"));
    if (FoeSelectionMaterialFinder.Succeeded())
        FoeSelectionMaterial = FoeSelectionMaterialFinder.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("FoeSelectionMaterial is not setted!!!"));

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetVisibility(false);
	DecalComponent->SetRelativeRotation(FRotator(-90.f, .0f, .0f));
}

void ASelectionIndicator::AttachToEntity(AActor* entity, float lifetime)
{

	// if (!entity)

    if (entity)
    {
        AttachToActor(entity, FAttachmentTransformRules::KeepRelativeTransform);
        SetActorHiddenInGame(false);
    }


}