// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableItem.h"
#include "../../../Characters/GB_BaseCharacter.h"
#include "../../Projectiles/GB_Projectile.h"

void AThrowableItem::Throw()
{
	AGB_BaseCharacter* CharacterOwner = StaticCast<AGB_BaseCharacter*>(GetOwner());

	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
	if (!IsValid(ProjectileClass))
	{
		return;
	}

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;

	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);

	FVector SpawnLocation = PlayerViewPoint + ViewDirection * 100.0f;
	AGB_Projectile* Projectile = GetWorld()->SpawnActor<AGB_Projectile>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator);
	if (IsValid(Projectile))
	{
		Projectile->SetOwner(GetOwner());
		Projectile->LaunchProjectile(ViewDirection);
	}
}
