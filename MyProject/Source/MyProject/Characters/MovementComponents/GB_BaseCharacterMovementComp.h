// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LedgeDetectorComponent.h"
#include "GB_BaseCharacterMovementComp.generated.h"

struct FMantlingMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector;
	FRotator InitialRotator = FRotator::ZeroRotator;

	FVector TargetLocation = FVector::ZeroVector;
	FRotator TartgetRotator = FRotator::ZeroRotator;

	FVector InitialAnimationLocation = FVector::ZeroVector;

	float Duration = 1.0f;
	float StartTime = 0.0f;

	UCurveVector* MantlingCurve;
};

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
	CMOVE_Ladder UMETA(DisplayName = "Ladder"),
	CMOVE_Max UMETA(Hidden)
};
/**
 * 
 */
UCLASS()
class MYPROJECT_API UGB_BaseCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	
	virtual void PhysicsRotation(float DeltaTime) override;

	FORCEINLINE bool IsSprinting() {return bIsSprinting;}
	virtual float GetMaxSpeed() const override;

	void StartSprint();
	void StopSprint();

	void StartMantle(const FMantlingMovementParameters& MantlingMovementParameters);
	void EndMantle();
	bool IsMantling() const;

	void AttachToLadder(const class ALadder* Ladder);

	float GetCharacterToCurrentLadderProjection(const FVector& Location) const;

	void DetachFromLadder();
	bool IsOnLadder() const;

	float GetLadderSpeedRatio() const;

	const class ALadder* GetCurrentLadder();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Swimming", meta=(ClampMin="0", UIMin="0"))
	float SwimmingCapsuleRadius = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Swimming", meta = (ClampMin = "0", UIMin = "0"))
	float SwimmingCapsuleHalfSize = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement | Sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SpintSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement | Sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float OutOfStaminaSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement | Ladder", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float ClimbingOnLadderMaxSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement | Ladder", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float LadderToCharacterOffset= 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement | Ladder", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxLadderTopOffset = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement | Ladder", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinLadderBottomOffset = 50.0f;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	void PhysMantling(float DeltaTime, int32 Iterations);
	void PhysLadder(float DeltaTime, int32 Iterations);

	class AGB_BaseCharacter* GetBaseCharacterOwner() const;

private:
	bool bIsSprinting;

	FMantlingMovementParameters CurrentMantlingParameters;

	FTimerHandle MantlingTimer;

	const ALadder* CurrentLadder = nullptr;
};
