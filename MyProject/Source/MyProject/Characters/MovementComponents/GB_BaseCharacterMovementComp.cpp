// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_BaseCharacterMovementComp.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

float UGB_BaseCharacterMovementComp::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();
	if (bIsSprinting)
	{
		Result = SpintSpeed;
	}
	return Result;
}

void UGB_BaseCharacterMovementComp::StartSprint()
{
	bIsSprinting = true;
	bForceMaxAccel = 1;
}

void UGB_BaseCharacterMovementComp::StopSprint()
{
	bIsSprinting = false;
	bForceMaxAccel = 0;
}

void UGB_BaseCharacterMovementComp::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (MovementMode == MOVE_Swimming)
	{
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(SwimmingCapsuleRadius, SwimmingCapsuleHalfSize);
	}
	else if (PreviousMovementMode == MOVE_Swimming)
	{
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), true);
	}
}
