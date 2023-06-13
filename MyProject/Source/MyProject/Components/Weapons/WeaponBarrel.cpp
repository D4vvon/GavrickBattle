// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBarrel.h"
#include "../../GavrickBattleTypes.h"
#include "DrawDebugHelpers.h"
#include "../../SubSystems/DebugSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "../../Actor/Projectiles/GB_Projectile.h"
#include "Components/DecalComponent.h"

void UWeaponBarrel::Shot(FVector ShotStart, FVector ShotDirection, float SpreadAngle)
{
	for (int i = 0; i < BulletsPerShot; i++)
	{
		ShotDirection += GetBulletSpreadOffset(FMath::RandRange(0.0f, SpreadAngle), ShotDirection.ToOrientationRotator());
		FVector MuzzleLocation = GetComponentLocation();
		FVector ShotEnd = (ShotStart + FiringRange * ShotDirection);
		//DrawDebugSphere(GetWorld(), MuzzleLocation, 10.0f, 24, FColor::Red, false, 1.0f);

#if ENABLE_DRAW_DEBUG
	UDebugSubsystem* DebugSubSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubSystem->IsCategoryEnabled(DebugCategoryRangeWeapon);
#else
	bool bIsDebugEnabled = false;
#endif
	
		switch (HitRegistration)
		{
		case EHitRegistrationType::HitScan:
			{
				bool bHasHit = HitScan(ShotStart, ShotEnd, ShotDirection);
				bIsTraceEnable = true;
				if (bIsDebugEnabled && bHasHit)
				{
					DrawDebugSphere(GetWorld(), ShotEnd, 10.0f, 24, FColor::Red, false, 1.0f);
				}
				break;
			}
		case EHitRegistrationType::Projectile:
			{
				bIsTraceEnable = false;
				LaunchProjectile(ShotStart, ShotDirection, ShotEnd);
				break;
			}
		default:
			break;
		}

		if (IsValid(MuzzleFlashFX))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashFX, MuzzleLocation, GetComponentRotation());
		}
		else if (IsValid(MuzzleFlashFX_Paricle))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashFX_Paricle, FTransform((GetComponentRotation() + FRotator(0.0f, 90.0f, 0.0f)), GetComponentLocation(), GetComponentScale()));
		}

		if (IsValid(TraceFX) && bIsTraceEnable)
		{
			UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, MuzzleLocation, GetComponentRotation());
			TraceFXComponent->SetVectorParameter(FXParameterTraceEnd, ShotEnd);
		}

		if (bIsDebugEnabled)
		{
			DrawDebugLine(GetWorld(), MuzzleLocation, ShotEnd, FColor::Red, false, 1.0f, 0, 3.0f);
		}
	}
}

bool UWeaponBarrel::HitScan(FVector ShotStart, OUT FVector& ShotEnd, FVector ShotDirection)
{
	FHitResult ShotResult;
	bool bHasHit = GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet);
	if (bHasHit)
	{
		ShotEnd = ShotResult.ImpactPoint;
		ProcessHit(ShotResult, ShotDirection);
	}	
	return bHasHit;
}

void UWeaponBarrel::LaunchProjectile(FVector ShotStart, FVector ShotDirection, FVector ShotEnd)
{
	AGB_Projectile* Projectile = GetWorld()->SpawnActor<AGB_Projectile>(ProjectileClass, ShotStart, ShotDirection.ToOrientationRotator());
	if (IsValid(Projectile))
	{
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, FString::Printf(TEXT("ShotDirection: %f, %f, %f"), ShotDirection.X, ShotDirection.Y, ShotDirection.Z), false);
		Projectile->SetOwner(GetOwningPawn());
		Projectile->OnProjectileHit.AddDynamic(this, &UWeaponBarrel::ProcessHit);
		Projectile->LaunchProjectile(ShotDirection.GetSafeNormal());
	}
}

FVector UWeaponBarrel::GetBulletSpreadOffset(float Angle, FRotator ShotRotation)
{
	float SpreadSize = FMath::Tan(Angle);
	float RotationAngle = FMath::RandRange(0.0f, 2 * PI);

	float SpreadY = FMath::Cos(RotationAngle);
	float SpreadZ = FMath::Sin(RotationAngle);

	FVector Result = (ShotRotation.RotateVector(FVector::UpVector) * SpreadZ + ShotRotation.RotateVector(FVector::RightVector) * SpreadY) * SpreadSize;

	return Result;
}

APawn* UWeaponBarrel::GetOwningPawn()
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!IsValid(PawnOwner))
	{
		PawnOwner = Cast<APawn>(GetOwner()->GetOwner());
	}
	return PawnOwner;
}

AController* UWeaponBarrel::GetController()
{
	APawn* PawnOwner = GetOwningPawn();
	return IsValid(PawnOwner) ? PawnOwner->GetController() : nullptr;
}

void UWeaponBarrel::ProcessHit(const FHitResult& HitResilt, const FVector& Direction)
{
	AActor* HitActor = HitResilt.GetActor();
	if (IsValid(HitActor))
	{
		FPointDamageEvent DamageEvent;
		DamageEvent.HitInfo = HitResilt;
		DamageEvent.ShotDirection = Direction;
		DamageEvent.DamageTypeClass = DamgeTypeClass;
		HitActor->TakeDamage(DamageAmount, DamageEvent, GetController(), GetOwner());
		//DrawDebugSphere(GetWorld(), HitResilt.ImpactPoint, 10.0f, 24, FColor::Red, false, 1.0f);
	}

	UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DefaultDecalInfo.DecalMaterial, DefaultDecalInfo.DecalSize, HitResilt.ImpactPoint, HitResilt.ImpactNormal.ToOrientationRotator());
	if (IsValid(DecalComponent))
	{
		DecalComponent->SetFadeScreenSize(0.0001f);
		DecalComponent->SetFadeOut(DefaultDecalInfo.DecalLifetime, DefaultDecalInfo.DecalFadeoutTime);
	}
}

