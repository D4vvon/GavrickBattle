// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "T72_Pawn_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UT72_Pawn_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimInstance")
	float TurretRotationSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimInstance")
	bool bIsTurretRotate = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimInstance")
	float MuzzleRotationSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimInstance")
	bool bIsMuzzleRotate = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimInstance")
	bool bIsAiming = false;

	UFUNCTION(BlueprintCallable)
	float RotateTurretRight(float CurrentRotation);
	UFUNCTION(BlueprintCallable)
	float RotateTurretLeft(float CurrentRotation);

	UFUNCTION(BlueprintCallable)
	float RotateMuzzleUp(float CurrentRotation);
	UFUNCTION(BlueprintCallable)
	float RotateMuzzleDown(float CurrentRotation);

private:
	TWeakObjectPtr<class AT72_Pawn> CachedT72_Pawn;
	
};
