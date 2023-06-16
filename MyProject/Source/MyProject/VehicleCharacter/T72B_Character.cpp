// Fill out your copyright notice in the Description page of Project Settings.


#include "T72B_Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/Components/BoxComponent.h"

AT72B_Character::AT72B_Character()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

void AT72B_Character::StartMoveForward(float Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Start Forward"));
	if ((!FMath::IsNearlyZero(Value, 1e-6f)))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, Value);
	}
}

void AT72B_Character::StartMoveRight(float Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Start Right"));
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		//FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		//FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
		//AddMovementInput(RightVector, Value);
		
		//BodyCollision->SetRelativeRotation(FRotator(0.0f, (BodyCollision->GetRelativeRotation().Yaw) * Value, 0.0f));

		AddMovementInput(GetActorRightVector(), Value);		
	}
}

void AT72B_Character::StartRotateTurret(float Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Start Turret"));
}

void AT72B_Character::StartMoveGun(float Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("StartMuzzle"));
}
