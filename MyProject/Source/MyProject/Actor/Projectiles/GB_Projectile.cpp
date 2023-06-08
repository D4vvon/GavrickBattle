// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGB_Projectile::AGB_Projectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision component"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SetRootComponent(CollisionComponent);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement component"));
	ProjectileMovementComp->InitialSpeed = 2000.0f;
}

void AGB_Projectile::LaunchProjectile(FVector Direction)
{
	ProjectileMovementComp->Velocity = Direction * ProjectileMovementComp->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
}

