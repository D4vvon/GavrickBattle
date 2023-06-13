// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponItem.h"
#include "../../../Components/Weapons/WeaponBarrel.h"
#include "../../../GavrickBattleTypes.h"
#include "../../../Characters/GB_BaseCharacter.h"
#include "Animation/AnimInstance.h"

ARangeWeaponItem::ARangeWeaponItem()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponBarell = CreateDefaultSubobject<UWeaponBarrel>(TEXT("WeaponBarell"));
	WeaponBarell->SetupAttachment(WeaponMesh, SocketWeaponMuzzle);

	ReticleType = EReticleType::Pistol;

	EquippedSocketName = SocketCharacterWeapon;
}

void ARangeWeaponItem::StartFire()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ShotTimer))
	{
		return;
	}

	bIsFiring = true;
	MakeShot();
}

void ARangeWeaponItem::StopFire()
{
	bIsFiring = false;
}

void ARangeWeaponItem::StartAim()
{
	bIsAiming = true;
}

void ARangeWeaponItem::StopAim()
{
	bIsAiming = false;
}

FTransform ARangeWeaponItem::GetForegripTransform() const
{
	return WeaponMesh->GetSocketTransform(SocketWeaponForegrip);
}

float ARangeWeaponItem::GetAimFOV()
{
	return AimFOV;
}

float ARangeWeaponItem::GetAimMovementMaxSpeed()
{
	return AimMovementMaxSpeed;
}

int32 ARangeWeaponItem::GetAmmo() const
{
	return Ammo;
}

int32 ARangeWeaponItem::GetMaxAmmo() const
{
	return MaxAmmo;
}

void ARangeWeaponItem::SetAmmo(int32 NewAmmo)
{
	Ammo = NewAmmo;
	if (OnAmmoChanged.IsBound())
	{
		OnAmmoChanged.Broadcast(Ammo);
	}
}

bool ARangeWeaponItem::CanShoot()
{
	return Ammo > 0;
}

void ARangeWeaponItem::StartReload()
{
	AGB_BaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	bIsReloading = true;
	if (IsValid(CharacterReloadMontage))
	{
		float MontageDuration = CharacterOwner->PlayAnimMontage(CharacterReloadMontage);
		PlayAnimMontage(WeaponReloadMontage);
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, [this]() { EndReload(true); }, MontageDuration, false);
	}
	else
	{
		EndReload(true);
	}
}

void ARangeWeaponItem::EndReload(bool bIsSuccess)
{
	if (!bIsReloading)
	{
		return;
	}

	AGB_BaseCharacter* CharacterOwner = GetCharacterOwner();

	if (!bIsSuccess)
	{
		if (IsValid(CharacterOwner))
		{
			CharacterOwner->StopAnimMontage(CharacterReloadMontage);
		}
		StopAnimMontage(WeaponReloadMontage, false);
	}

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);

	bIsReloading = false;
	if (bIsSuccess && OnReloadComplete.IsBound())
	{
		OnReloadComplete.Broadcast();
	}
}

EAmmunitionType ARangeWeaponItem::GetAmmoType() const
{
	return AmmoType;
}

EReticleType ARangeWeaponItem::GetReticleType() const
{
	return bIsAiming ? AimReticleType : Super::GetReticleType();
}

void ARangeWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	SetAmmo(MaxAmmo);
}

float ARangeWeaponItem::GetCurrentBulletSpread()
{
	float AngleInDegress = bIsAiming ? AimSpreadAngle : SpreadAngle;
	return FMath::DegreesToRadians(AngleInDegress); 
}

void ARangeWeaponItem::MakeShot()
{
	AGB_BaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
	{
		return;
	}
	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();

	if (!CanShoot())
	{
		StopFire();
		if (Ammo == 0 && bAutoReload)
		{
			CharacterOwner->ReloadCurrebtRangeWeapon();
		}
		return;
	}

	CharacterOwner->PlayAnimMontage(CharacterFireMontage);
	if (IsValid(WeaponFireMontage))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Shot"));
		PlayAnimMontage(WeaponFireMontage);
	}

	if (!IsValid(Controller))
	{
		return;
	}
	EndReload(false);

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;
	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FVector ShotStart = GetActorLocation();
	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);

	WeaponBarell->Shot(PlayerViewPoint, ViewDirection, GetCurrentBulletSpread());
	//WeaponBarell->Shot(ShotStart, ViewDirection);
	SetAmmo(Ammo - 1);

	GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ARangeWeaponItem::OnShotTimerElapsed, GetShotTimerInterval(), false);
}

float ARangeWeaponItem::GetShotTimerInterval()
{
	return 60.0f / RateOfFire;
}

float ARangeWeaponItem::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	float Result = 0.0f;
	if (IsValid(WeaponAnimInstance))
	{
		Result = WeaponAnimInstance->Montage_Play(AnimMontage);
	}
	return Result;
}

void ARangeWeaponItem::StopAnimMontage(UAnimMontage* AnimMontage, float BlendOutTime /*= 0.0f*/)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (IsValid(WeaponAnimInstance))
	{
		WeaponAnimInstance->Montage_Stop(BlendOutTime, AnimMontage);	
	}
}

void ARangeWeaponItem::OnShotTimerElapsed()
{
	if (!bIsFiring)
	{
		return;
	}

	switch (WeaponFireMode)
	{
	case EWeaponFireMode::Single:
	{
		StopFire();
		break;
	}		
	case EWeaponFireMode::FullAuto:
	{
		MakeShot();
	}

	default:
		break;
	}
}
