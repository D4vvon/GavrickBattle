// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_BasePlanePawn.h"
#include "../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Components/Su33MovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

AGB_BasePlanePawn::AGB_BasePlanePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	MoverComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Mover Component"));
	SetRootComponent(MoverComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetCollisionProfileName(FName("Pawn"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, USu33MovementComponent>(TEXT("Movement Component"));
	MovementComponent->SetUpdatedComponent(MoverComponent);

}

void AGB_BasePlanePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Roll", this, &AGB_BasePlanePawn::AddRoll);
	PlayerInputComponent->BindAxis("Pitch", this, &AGB_BasePlanePawn::AddPitch);
	PlayerInputComponent->BindAxis("Yaw", this, &AGB_BasePlanePawn::AddYaw);

	PlayerInputComponent->BindAxis("Accelaration", this, &AGB_BasePlanePawn::Accelerate);
	PlayerInputComponent->BindAxis("Decelaration", this, &AGB_BasePlanePawn::Decelarate);
}

void AGB_BasePlanePawn::AddRoll(float Value)
{
	MovementComponent->AddInputVector(Value * FVector::ForwardVector);
}

void AGB_BasePlanePawn::AddPitch(float Value)
{
	MovementComponent->AddInputVector(Value * FVector::RightVector);
}

void AGB_BasePlanePawn::AddYaw(float Value)
{
	//RotationInput.Z = FMath::Clamp((RotationInput.Z + Value), -1.0f, 1.0f);
	MovementComponent->AddInputVector(Value * FVector::UpVector);
}


void AGB_BasePlanePawn::Accelerate(float Value)
{
	GetSu33Movement()->SetAcceleration(Value);
}

void AGB_BasePlanePawn::Decelarate(float Value)
{	
	GetSu33Movement()->SetDeceleration(Value);
}

class UBoxComponent* AGB_BasePlanePawn::GetMoverComp()
{
	return MoverComponent;
}

USkeletalMeshComponent* AGB_BasePlanePawn::GetSkelMesh()
{
	return SkeletalMeshComponent;
}

USu33MovementComponent* AGB_BasePlanePawn::GetSu33Movement()
{
	return StaticCast<USu33MovementComponent*>(MovementComponent);
}

void AGB_BasePlanePawn::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(FrontWheel))
	{
	PlayAnimMontage(FrontWheel);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Your Message"));
	}
	PlayAnimMontage(RightWheel);
	PlayAnimMontage(LeftWheel);
}

float AGB_BasePlanePawn::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* PlaneAnimInstance = SkeletalMeshComponent->GetAnimInstance();
	float Result = 0.0f;
	if (IsValid(PlaneAnimInstance))
	{
		Result = PlaneAnimInstance->Montage_Play(AnimMontage);
	}
	return Result;
}
