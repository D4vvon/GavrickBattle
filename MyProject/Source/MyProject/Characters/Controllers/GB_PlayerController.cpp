// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_PlayerController.h"
#include "../GB_BaseCharacter.h"

void AGB_PlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedBaseCharacter = Cast<AGB_BaseCharacter>(InPawn);
}

void AGB_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AGB_PlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGB_PlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AGB_PlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AGB_PlayerController::LookUp);

	InputComponent->BindAction("InteractWithLadder", EInputEvent::IE_Pressed, this, &AGB_PlayerController::InteractWithLadder);
	InputComponent->BindAction("Mantle", EInputEvent::IE_Pressed, this, &AGB_PlayerController::Mantle);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AGB_PlayerController::Jump);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AGB_PlayerController::ChangeCrouchState);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AGB_PlayerController::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AGB_PlayerController::StopSprint);

	InputComponent->BindAxis("SwimForward", this, &AGB_PlayerController::SwimForward);
	InputComponent->BindAxis("SwimRight", this, &AGB_PlayerController::SwimRight);
	InputComponent->BindAxis("SwimUp", this, &AGB_PlayerController::SwimUp);

	InputComponent->BindAxis("ClimbLadderUp", this, &AGB_PlayerController::ClimbLadderUp);

	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AGB_PlayerController::Fire);
}

void AGB_PlayerController::MoveForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(Value);
	}
}

void AGB_PlayerController::MoveRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(Value);
	}
}

void AGB_PlayerController::Turn(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(Value);
	}
}

void AGB_PlayerController::LookUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(Value);
	}
}

void AGB_PlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

void AGB_PlayerController::StartSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartSprint();
	}
}

void AGB_PlayerController::StopSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopSprint();
	}
}

void AGB_PlayerController::SwimForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimForward(Value);
	}
}

void AGB_PlayerController::SwimRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimRight(Value);
	}
}

void AGB_PlayerController::SwimUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimUp(Value);
	}
}

void AGB_PlayerController::Mantle()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Mantle(false);
	}
}

void AGB_PlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}

void AGB_PlayerController::ClimbLadderUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ClimbLadderUp(Value);
	}
}

void AGB_PlayerController::InteractWithLadder()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->InteractWithLadder();
	}
}

void AGB_PlayerController::Fire()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Fire();
	}
}
