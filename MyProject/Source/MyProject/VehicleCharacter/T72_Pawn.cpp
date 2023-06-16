// Fill out your copyright notice in the Description page of Project Settings.


#include "T72_Pawn.h"
#include "../GavrickBattleTypes.h"
#include "../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"
#include "../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/Components/BoxComponent.h"
#include "../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/Engine/SkeletalMesh.h"
#include "../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/Components/SceneComponent.h"
#include "../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/Engine/CollisionProfile.h"
#include "Components/VehiclePawnMovementComp.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/T72_MovementComponent.h"
#include "Components/CapsuleComponent.h"

AT72_Pawn::AT72_Pawn()
{	
	this->SetActorEnableCollision(true);
	PrimaryActorTick.bCanEverTick = true; 

	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Body Collision"));
	//RootComponent = BodyCollision;
	SetRootComponent(BodyCollision);

	TurretCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Turret Collision"));
	TurretCollision->SetupAttachment(BodyCollision);

	MuzzleCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Muzzle Collision"));
	MuzzleCollision->SetupAttachment(TurretCollision);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(MuzzleCollision);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	AimingCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Aiming Camera"));
	AimingCamera->SetupAttachment(MuzzleCollision);

	MovementComponent = CreateDefaultSubobject<UT72_MovementComponent>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(BodyCollision);

}

void AT72_Pawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateOrientation(DeltaSeconds);
	UpdateVelocityAndPosition(DeltaSeconds);
}

void AT72_Pawn::BeginPlay()
{
	Super::BeginPlay();
}

void AT72_Pawn::StartAiming()
{
	if (!bIsAiming)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Aimed"));
		Camera->SetActive(false);
		AimingCamera->SetActive(true);
		bIsAiming = true;
	}
}

void AT72_Pawn::StopAiming()
{
	if (bIsAiming)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("DisAimed"));
		AimingCamera->SetActive(false);
		Camera->SetActive(true);
		bIsAiming = false;
		
	}
}

bool AT72_Pawn::GetIsAiming()
{
	return bIsAiming;
}

/*
void AT72_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("StartMoveForward", this, &AT72_Pawn::StartMoveForward);
	PlayerInputComponent->BindAxis("StartMoveRight", this, &AT72_Pawn::StartMoveRight);
	PlayerInputComponent->BindAxis("StartRotateTurret", this, &AT72_Pawn::StartRotateTurret);
	PlayerInputComponent->BindAxis("StartMoveGun", this, &AT72_Pawn::StartMoveMuzzle);
}*/

void AT72_Pawn::StartMoveForward(float Value)
{
	if (Value > 0)
	{
		bIsMoving = true;
		bIsMoveForward = 1;
	}
	else if (Value < 0)
	{	
		bIsMoving = true;
		bIsMoveForward = -1;
	}
	else
	{
		bIsMoving = false;
	}
}

void AT72_Pawn::StartMoveRight(float Value)
{
	if (Value != 0)
	{
		RotationBodyInput.Y = FMath::Clamp(RotationBodyInput.Y + Value, -1.0f, 1.0f);
		bIsRotatingBody = true;
	}
	else
	{
		bIsRotatingBody = false;
	}
}

void AT72_Pawn::StartRotateTurret(float Value)
{
	if (Value != 0)
	{
		if (Value > 0)
		{
			Value = 1;
		}
		else if (Value < 0)
		{
			Value = -1;
		}
		RotationTurretInput.Y = FMath::Clamp(RotationTurretInput.Y + Value, -1.0f, 1.0f);
		bIsRotatingTurret = true;
		SetTurretDirection(Value);
	}
	else 
	{
		bIsRotatingTurret = false;
	}
}

void AT72_Pawn::StartMoveMuzzle(float Value)
{
	if (Value != 0)
	{
		if (Value < 0)
		{
			Value = 1;
		}
		else if (Value > 0)
		{
			Value = -1;
		}
		RotationMuzzleInput.X = FMath::Clamp(RotationTurretInput.X + Value, -1.0f, 1.0f);
		bIsRotatingMuzzle = true;
		SetMuzzleDirection(Value);
	}
	else
	{
		bIsRotatingMuzzle = false;
	}
}

float AT72_Pawn::GetTurretRotationSpeed()
{
	return TurretRotationSpeed;
}

float AT72_Pawn::GetMuzzleRotationSpeed()
{
	return MuzzleRotationSpeed;
}

bool AT72_Pawn::GetIsTurretRotate()
{
	return bIsRotatingTurret;
}

bool AT72_Pawn::GetIsMuzzleRotate()
{
	return bIsRotatingMuzzle;
}

/*
bool AT72_Pawn::GetIsRotationTurretWhileRotateBody()
{
	if (bIsRotatingBody && bIsRotatingTurret)
	{
		return true;
	}
	else
	{
		return false;
	}
}*/

void AT72_Pawn::SetTurretDirection(float NewDirection)
{
	TurretDirection = NewDirection;
}

float AT72_Pawn::GetTurretDiretion()
{
	return TurretDirection;
}

void AT72_Pawn::SetMuzzleDirection(float NewDirection)
{
	MuzzleDirection = NewDirection;
}

float AT72_Pawn::GetMuzzleDirection()
{
	return MuzzleDirection;
}

//X - UpDown, Y -LeftRight, Z - Rotate
void AT72_Pawn::UpdateOrientation(float DeltaTime)
{

//Body
	FVector RotationAnglesBody = RotationBodyInput * BodyRotationSpeed * DeltaTime;

	FQuat RotationPitchBody(GetActorUpVector(), FMath::DegreesToRadians(RotationAnglesBody.Y));

	FQuat NewRotationBody = RotationPitchBody * GetActorRotation().Quaternion();

	SetActorRotation(NewRotationBody);
	RotationBodyInput = FVector::ZeroVector;
//~Body
}

void AT72_Pawn::UpdateVelocityAndPosition(float DelatTime)
{	
	FVector Velocity;

	if (bIsMoving)
	{
		Velocity = GetActorForwardVector() * Speed * bIsMoveForward;
		BodyCollision->ComponentVelocity = Velocity;
		//BodyCollision->MoveComponent(Velocity * DelatTime, GetActorRotation(), false);
	}
	else
	{
		BodyCollision->ComponentVelocity = FVector::ZeroVector;
		//BodyCollision->MoveComponent(FVector::ZeroVector, GetActorRotation(), false);
	}

	FVector Delta = Velocity * DelatTime;
	FQuat Rot = MovementComponent->UpdatedComponent->GetComponentQuat();
	MovementComponent->MoveUpdatedComponent(Delta, Rot, true);
}

