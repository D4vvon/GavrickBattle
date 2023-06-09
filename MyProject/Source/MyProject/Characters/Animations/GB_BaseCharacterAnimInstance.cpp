// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_BaseCharacterAnimInstance.h"
#include "../GB_BaseCharacter.h"
#include "../MovementComponents/GB_BaseCharacterMovementComp.h"
#include "../CharacterComponents/CharacterAttributesComponent.h"
#include "../CharacterComponents/CharacterEquipmentComponent.h"
#include "../../Actor/Equipment/Weapons/RangeWeaponItem.h"

void UGB_BaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AGB_BaseCharacter>(), TEXT("UGB_BaseCharacterAnimInstance::NativeBeginPlay() can used only with AGB_BaseCharacter"))
	CachedBaseCharacter = StaticCast<AGB_BaseCharacter*>(TryGetPawnOwner());
}

void UGB_BaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}

	UGB_BaseCharacterMovementComp* CharacterMovementComponent = CachedBaseCharacter->GetBaseCharacterMovementComp();
	UCharacterAttributesComponent* CharacterAttributeComponent = CachedBaseCharacter->GetCharacterAttributeConponent();
	Speed = CharacterMovementComponent->Velocity.Size();
	bIsFalling = CharacterMovementComponent->IsFalling();
	bIsCrouching = CharacterMovementComponent->IsCrouching();

	bIsSprinting = CharacterMovementComponent->IsSprinting();

	bIsSwimming = CharacterMovementComponent->IsSwimming();

	bIsOnLadder = CharacterMovementComponent->IsOnLadder();
	if (bIsOnLadder)
	{
		LadderSpeedRatio = CharacterMovementComponent->GetLadderSpeedRatio();
	}

	bIsStrafing = !CharacterMovementComponent->bOrientRotationToMovement;
	Direction = CalculateDirection(CharacterMovementComponent->Velocity, CachedBaseCharacter->GetActorRotation());

	AimRotation = CachedBaseCharacter->GetBaseAimRotation();

	bIsOutOfStamina = CharacterAttributeComponent->IsOutOfStamina() && !CachedBaseCharacter->GetIsSprintRequest();

	const UCharacterEquipmentComponent* CharacterEquipment = CachedBaseCharacter->GetCharacterEquipmentComponent();
	CurrentEquippedItemType = CharacterEquipment->GetCurrentEquippedItemType();

	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipment->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		ForegripSocketTransform = CurrentRangeWeapon->GetForegripTransform();
	}

	bIsAiming = CachedBaseCharacter->IsAiming();

}
