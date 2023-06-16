// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB_BaseTrackVehicleCharacter.h"
#include "T72B_Character.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYPROJECT_API AT72B_Character : public AGB_BaseTrackVehicleCharacter
{
	GENERATED_BODY()
	
public:
	AT72B_Character();

	virtual void StartMoveForward(float Value) override;
	virtual void StartMoveRight(float Value) override;

	virtual void StartRotateTurret(float Value) override;
	virtual void StartMoveGun(float Value) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement")
	float BodyRotationSpeed = 10.0f;
};
