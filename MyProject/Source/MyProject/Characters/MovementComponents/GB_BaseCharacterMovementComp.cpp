// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_BaseCharacterMovementComp.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "../../Actor/Environment/Ladder.h"
#include "../GB_BaseCharacter.h"

void UGB_BaseCharacterMovementComp::PhysicsRotation(float DeltaTime)
{
	if (IsOnLadder())
	{
		return;
	}
	Super::PhysicsRotation(DeltaTime);
}

float UGB_BaseCharacterMovementComp::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();
	if (bIsSprinting)
	{
		Result = SpintSpeed;
	}
	else if (IsOnLadder())
	{
		Result = ClimbingOnLadderMaxSpeed;
	}
	else if (GetBaseCharacterOwner()->GetCurrentStamina() <= 10.0f)
	{
		Result = OutOfStaminaSpeed;
	}
	else if (GetBaseCharacterOwner()->IsAiming())
	{
		Result = GetBaseCharacterOwner()->GetAimingMovementSpeed();
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

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_Ladder)
	{
		CurrentLadder = nullptr;
	}

	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
			case (uint8)ECustomMovementMode::CMOVE_Mantling:
			{
				GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &UGB_BaseCharacterMovementComp::EndMantle, CurrentMantlingParameters.Duration, false);
			}
		default:
			break;
		}
	}
}

void UGB_BaseCharacterMovementComp::PhysCustom(float deltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case (uint8)ECustomMovementMode::CMOVE_Mantling:
	{
		PhysMantling(deltaTime, Iterations);

		break;
	}
	case (uint8)ECustomMovementMode::CMOVE_Ladder:
	{
		PhysLadder(deltaTime, Iterations);

		break;
	}
	default:
		break;
	}

	Super::PhysCustom(deltaTime, Iterations);
}

void UGB_BaseCharacterMovementComp::PhysMantling(float DeltaTime, int32 Iterations)
{
	float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) + CurrentMantlingParameters.StartTime;

	FVector MantlingCurveValue = CurrentMantlingParameters.MantlingCurve->GetVectorValue(ElapsedTime);

	float PositionAlpha = MantlingCurveValue.X;
	float XYCoreectionAlpha = MantlingCurveValue.Y;
	float ZCorrectionAlpha = MantlingCurveValue.Z;

	FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingParameters.InitialLocation, CurrentMantlingParameters.InitialAnimationLocation, XYCoreectionAlpha);
	CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingParameters.InitialLocation.Z, CurrentMantlingParameters.InitialAnimationLocation.Z, ZCorrectionAlpha);

	FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, CurrentMantlingParameters.TargetLocation, PositionAlpha);
	FRotator NewRotation = FMath::Lerp(CurrentMantlingParameters.TartgetRotator, CurrentMantlingParameters.TartgetRotator, PositionAlpha);

	FVector Delta = NewLocation - GetActorLocation();

	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, NewRotation, false, Hit);
}

void UGB_BaseCharacterMovementComp::PhysLadder(float DeltaTime, int32 Iterations)
{
	CalcVelocity(DeltaTime, 1.0f, false, 2048.0f);
	FVector Delta = Velocity * DeltaTime;

	if (HasAnimRootMotion())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), false, Hit);
		return;
	}

	FVector NewPos = GetActorLocation() + Delta;
	float NewPosProjection = GetCharacterToCurrentLadderProjection(NewPos);

	if (NewPosProjection < MinLadderBottomOffset)
	{
		DetachFromLadder();
		return;
	}
	else if (NewPosProjection > (CurrentLadder->GetLadderHeight() - MaxLadderTopOffset))
	{
		GetBaseCharacterOwner()->Mantle(true);
		return;
	}

	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), false, Hit);
}

AGB_BaseCharacter* UGB_BaseCharacterMovementComp::GetBaseCharacterOwner() const
{
	return StaticCast<AGB_BaseCharacter*>(CharacterOwner);
}

void UGB_BaseCharacterMovementComp::StartMantle(const FMantlingMovementParameters& MantlingMovementParameters)
{
	CurrentMantlingParameters = MantlingMovementParameters;
	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Mantling);
}

void UGB_BaseCharacterMovementComp::EndMantle()
{
	SetMovementMode(EMovementMode::MOVE_Walking);
}

bool UGB_BaseCharacterMovementComp::IsMantling() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Mantling;
}

void UGB_BaseCharacterMovementComp::AttachToLadder(const class ALadder* Ladder)
{
	CurrentLadder = Ladder;

	FRotator TargetOrintationRotation = CurrentLadder->GetActorForwardVector().ToOrientationRotator();
	TargetOrintationRotation.Yaw += 180.0f;

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderForwardVector = CurrentLadder->GetActorForwardVector();
	float Projection = GetCharacterToCurrentLadderProjection(GetActorLocation());


	FVector NewCharacterLocation = CurrentLadder->GetActorLocation() + Projection * LadderUpVector + LadderToCharacterOffset * LadderForwardVector;
	//FVector NewCharacterLocationFixed = FVector(CurrentLadder->GetActorLocation().X, CurrentLadder->GetActorLocation().Y - 60.0f, (CurrentLadder->GetActorLocation() + Projection * LadderUpVector + LadderToCharacterOffset * LadderForwardVector).Z);
	if (CurrentLadder->GetIsOnTop())
	{
		NewCharacterLocation = CurrentLadder->GetAttachFromTopAnimMontageStartingLocation();
	}

	GetOwner()->SetActorLocation(NewCharacterLocation);
	GetOwner()->SetActorRotation(TargetOrintationRotation);

	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Ladder);
}

float UGB_BaseCharacterMovementComp::GetCharacterToCurrentLadderProjection(const FVector& Location) const
{
	checkf(IsValid(CurrentLadder), TEXT("float UGB_BaseCharacterMovementComp::GetCharacterToCurrentLadderProjection() can use with ladder"))

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderCharacterDistance = Location - CurrentLadder->GetActorLocation();
	return FVector::DotProduct(LadderUpVector, LadderCharacterDistance);
}

void UGB_BaseCharacterMovementComp::DetachFromLadder()
{
	SetMovementMode(MOVE_Falling);
}

bool UGB_BaseCharacterMovementComp::IsOnLadder() const
{
	//return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Ladder;
	return CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Ladder;
}

float UGB_BaseCharacterMovementComp::GetLadderSpeedRatio() const
{
	checkf(IsValid(CurrentLadder), TEXT("float UGB_BaseCharacterMovementComp::GetCharacterToCurrentLadderProjection() can use with ladder"))
	
	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	return FVector::DotProduct(LadderUpVector, Velocity) / ClimbingOnLadderMaxSpeed;
}

const class ALadder* UGB_BaseCharacterMovementComp::GetCurrentLadder()
{
	return CurrentLadder;
}


