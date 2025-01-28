// Fill out your copyright notice in the Description page of Project Settings.

#include "Units/BaseUnit.h"

// Sets default values
ABaseUnit::ABaseUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName(FName("Pawn"));
	BoxComponent->SetCanEverAffectNavigation(false);

	SetRootComponent(BoxComponent);

	EntityComponent = CreateDefaultSubobject<UEntityComponent>(TEXT("EntityComponent"));
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	SelectionComponent = CreateDefaultSubobject<USelectionComponent>(TEXT("SelectionComponent"));

	ControlComponent = CreateDefaultSubobject<UControlComponent>(TEXT("ControlComponent"));
	
	FastPhysicsEngine = CreateDefaultSubobject<UFastPhysicsEngine>(TEXT("FastPhysicsEngine"));
	FastPhysicsEngine->SetAutoActivate(true);

	HealthComponent->OnHealthZero.AddDynamic(this, &ABaseUnit::OnUnitDeath);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->SetVisibility(false);
	DecalComponent->SetRelativeRotation(FRotator(-90.f, .0f, .0f));

	SelectionComponent->SetDecalComponent(DecalComponent);

	SetCanAffectNavigationGeneration(false);
}

// Called when the game starts or when spawned
void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseUnit::OnUnitDeath()
{
	ControlComponent->SetUncontrollable();
	EntityComponent->KillEntity();
}
