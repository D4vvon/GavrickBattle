// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GB_BaseCharacterMovementComp.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UGB_BaseCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	FORCEINLINE bool IsSprinting() {return bIsSprinting;}
	virtual float GetMaxSpeed() const override;

	void StartSprint();
	void StopSprint();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Swimming", meta=(ClampMin="0", UIMin="0"))
	float SwimmingCapsuleRadius = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Swimming", meta = (ClampMin = "0", UIMin = "0"))
	float SwimmingCapsuleHalfSize = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement | Sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SpintSpeed = 1200.0f;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
private:
	bool bIsSprinting;
};
