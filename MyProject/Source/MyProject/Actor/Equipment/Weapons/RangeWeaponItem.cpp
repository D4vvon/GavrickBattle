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
}

void ARangeWeaponItem::Fire()
{
	AGB_BaseCharacter* CharacterOwner = StaticCast<AGB_BaseCharacter*>(GetOwner());
	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();

	CharacterOwner->PlayAnimMontage(CharacterFireMontage);
	PlayeAnimMontage(WeaponFireMontage);

	if (!IsValid(Controller))
	{
		return;
	}

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;
	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);

	WeaponBarell->Shot(PlayerViewPoint, ViewDirection, Controller);
}

float ARangeWeaponItem::PlayeAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	return WeaponAnimInstance->Montage_Play(AnimMontage);
}
