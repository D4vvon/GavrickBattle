// Fill out your copyright notice in the Description page of Project Settings.


#include "T72_Pawn_AnimInstance.h"
#include "../T72_Pawn.h"

void UT72_Pawn_AnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	checkf(TryGetPawnOwner()->IsA<AT72_Pawn>(), TEXT("Only with T72_Pawn"));
	CachedT72_Pawn = StaticCast<AT72_Pawn*>(TryGetPawnOwner());

}

void UT72_Pawn_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedT72_Pawn.IsValid())
	{
		return;
	}

	//Turret Mesh rotate
	if (bIsTurretRotate)
	{
		if (CachedT72_Pawn->GetTurretDiretion() > 0.0f)
		{	
			TurretRotationSpeed = RotateTurretRight(TurretRotationSpeed);
		}
		else if (CachedT72_Pawn->GetTurretDiretion() < 0.0f)
		{
			TurretRotationSpeed = RotateTurretLeft(TurretRotationSpeed);
		}
	}
	//~Turret Mesh rotate

	//Muzzle Mesh rotate
	if (bIsMuzzleRotate)
	{
		if (CachedT72_Pawn->GetMuzzleDirection() > 0.0f)
		{
			MuzzleRotationSpeed = RotateMuzzleUp(MuzzleRotationSpeed);
		}
		else if (CachedT72_Pawn->GetMuzzleDirection() < 0.0f)
		{
			MuzzleRotationSpeed = RotateMuzzleDown(MuzzleRotationSpeed);
		}
	}

	//~MuzzleMesh rotate

	bIsMuzzleRotate = CachedT72_Pawn->GetIsMuzzleRotate();
	bIsTurretRotate = CachedT72_Pawn->GetIsTurretRotate();

	bIsAiming = CachedT72_Pawn->GetIsAiming();
}

float UT72_Pawn_AnimInstance::RotateTurretRight(float CurrentRotation)
{
	//CurrentRotation += 0.287;
	if (!bIsAiming)
	{
		CurrentRotation += 0.15;
	}
	else
	{
		CurrentRotation += 0.03;
	}
	return CurrentRotation;
}

float UT72_Pawn_AnimInstance::RotateTurretLeft(float CurrentRotation)
{
	//CurrentRotation -= 0.287;
	if (!bIsAiming)
	{
		CurrentRotation -= 0.15;
	}
	else
	{
		CurrentRotation -= 0.03;
	}
	return CurrentRotation;
}

float UT72_Pawn_AnimInstance::RotateMuzzleUp(float CurrentRotation)
{
	//CurrentRotation -=0.21525f /*0.287f * 0.75f*/;
	if (!bIsAiming)
	{
		CurrentRotation -= 0.1;
	}
	else
	{
		CurrentRotation -= 0.02;
	}
	if (CurrentRotation < -7.89f)
	{
		CurrentRotation = -7.89f;
	}
	return CurrentRotation;
}

float UT72_Pawn_AnimInstance::RotateMuzzleDown(float CurrentRotation)
{
	//CurrentRotation +=0.21525f /*0.287f * 0.75f*/;
	if (!bIsAiming)
	{
		CurrentRotation += 0.1;
	}
	else
	{
		CurrentRotation +=0.02;
	}
	if (CurrentRotation > 10.89f)
	{
		CurrentRotation = 10.89f;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rotation = %f"), CurrentRotation));
	return CurrentRotation;
}
