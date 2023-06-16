// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_BaseTrackVehicleCharacter.h"
#include "../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/Components/BoxComponent.h"
#include "../GavrickBattleTypes.h"



AGB_BaseTrackVehicleCharacter::AGB_BaseTrackVehicleCharacter()
{
	//BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Body Collision"));
	//BodyCollision->SetCollisionProfileName(CollisionProfilePawn);
	//BodyCollision->SetupAttachment(RootComponent);

	//TurretCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Turret Collision"));
	//TurretCollision->SetCollisionProfileName(CollisionProfilePawn);
	//TurretCollision->SetupAttachment(RootComponent);

	//MuzzleCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Muzzle Collision"));
	//MuzzleCollision->SetCollisionProfileName(CollisionProfilePawn);
	//MuzzleCollision->SetupAttachment(TurretCollision);
}

void AGB_BaseTrackVehicleCharacter::BeginPlay()
{
	Super::BeginPlay();
}

