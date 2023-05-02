// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values


void AGB_BaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}
