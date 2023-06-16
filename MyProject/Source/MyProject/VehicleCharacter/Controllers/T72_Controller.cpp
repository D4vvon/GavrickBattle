// Fill out your copyright notice in the Description page of Project Settings.


#include "T72_Controller.h"
#include "../T72_Pawn.h"
#include "Camera/CameraComponent.h"

void AT72_Controller::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedT72_Pawn = Cast<AT72_Pawn>(InPawn);
}

void AT72_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("StartMoveForward", this, &AT72_Controller::StartMoveForward);
	InputComponent->BindAxis("StartMoveRight", this, &AT72_Controller::StartMoveRight);
	InputComponent->BindAxis("StartRotateTurret", this, &AT72_Controller::StartRotateTurret);
	InputComponent->BindAxis("StartMoveGun", this, &AT72_Controller::StartMoveMuzzle);

	InputComponent->BindAction("StartTankAiming", EInputEvent::IE_Pressed, this, &AT72_Controller::StartAiming);
	InputComponent->BindAction("StopTankAiming", EInputEvent::IE_Released, this, &AT72_Controller::StopAiming);

}

void AT72_Controller::StartMoveForward(float Value)
{
	if (CachedT72_Pawn.IsValid())
	{
		CachedT72_Pawn->StartMoveForward(Value);
	}
}

void AT72_Controller::StartMoveRight(float Value)
{
	if (CachedT72_Pawn.IsValid())
	{
		CachedT72_Pawn->StartMoveRight(Value);
	}
}

void AT72_Controller::StartRotateTurret(float Value)
{
	if (CachedT72_Pawn.IsValid())
	{
		CachedT72_Pawn->StartRotateTurret(Value);
	}
}

void AT72_Controller::StartMoveMuzzle(float Value)
{
	if (CachedT72_Pawn.IsValid())
	{
		CachedT72_Pawn->StartMoveMuzzle(Value);
	}
}

void AT72_Controller::StartAiming()
{
	if (CachedT72_Pawn.IsValid())
	{
		CachedT72_Pawn->StartAiming();
	}
}

void AT72_Controller::StopAiming()
{
	if (CachedT72_Pawn.IsValid())
	{
		CachedT72_Pawn->StopAiming();
	}
}
