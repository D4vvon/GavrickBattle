// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MovementComponents/GB_BaseCharacterMovementComp.h"
#include "MovementComponents/LedgeDetectorComponent.h"

AGB_BaseCharacter::AGB_BaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGB_BaseCharacterMovementComp>(ACharacter::CharacterMovementComponentName))
{
	GB_BaseCharacterMovementComp = StaticCast<UGB_BaseCharacterMovementComp*>(GetCharacterMovement());
	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));
}

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

void AGB_BaseCharacter::StartSprint()
{
	bIsSprintRequested = true;
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void AGB_BaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

void AGB_BaseCharacter::Mantle()
{
	FLedgeDescription LedgeDescription;
	if (LedgeDetectorComponent->DetectLedge(LedgeDescription))
	{
		
	}
}

void AGB_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryChangeSprintState();
}

void AGB_BaseCharacter::OnSprintStart_Implementation()
{

}

void AGB_BaseCharacter::OnSprintEnd_Implementation()
{

}

bool AGB_BaseCharacter::CanSprint()
{
	return true;
}

void AGB_BaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && !GB_BaseCharacterMovementComp->IsSprinting() && CanSprint())
	{
		GB_BaseCharacterMovementComp->StartSprint();
		OnSprintStart();
	}

	if (!bIsSprintRequested && GB_BaseCharacterMovementComp->IsSprinting())
	{
		GB_BaseCharacterMovementComp->StopSprint();
		OnSprintEnd();
	}
}
