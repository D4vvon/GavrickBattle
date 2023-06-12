// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableItem.h"
#include "../../../Characters/GB_BaseCharacter.h"
#include "../../Projectiles/GB_Projectile.h"
#include "../../../GavrickBattleTypes.h"

void AThrowableItem::Throw()
{
	AGB_BaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
	if (!IsValid(ProjectileClass))
	{
		return;
	}

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;

	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FTransform PlayerViewTransform(PlayerViewRotation, PlayerViewPoint);

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);
	FVector ViewUpVector = PlayerViewRotation.RotateVector(FVector::UpVector);

	FVector LaunchDirection = ViewDirection + FMath::Tan(FMath::DegreesToRadians(ThrowAngle)) * ViewUpVector;

	FVector ThrowableSocketLocation = CharacterOwner->GetMesh()->GetSocketLocation(SocketCharacterThrowable);
	FVector SocketInViewSpace = PlayerViewTransform.InverseTransformPosition(ThrowableSocketLocation);

	//FVector SpawnLocation = PlayerViewPoint + ViewDirection * SocketInViewSpace.X;
	FVector SpawnLocation = PlayerViewPoint + ViewDirection + FVector(120.0f, 0.0f, 0.0f);
	AGB_Projectile* Projectile = GetWorld()->SpawnActor<AGB_Projectile>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator);
	if (IsValid(Projectile))
	{
		Projectile->SetOwner(GetOwner());
		Projectile->LaunchProjectile(LaunchDirection.GetSafeNormal());
	}
}
