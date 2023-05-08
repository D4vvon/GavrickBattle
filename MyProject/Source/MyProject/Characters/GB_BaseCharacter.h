// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GB_BaseCharacter.generated.h"

class UGB_BaseCharacterMovementComp;

UCLASS(Abstract, NotBlueprintable)
class MYPROJECT_API AGB_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGB_BaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};

	virtual void ChangeCrouchState();
	
	virtual void StartSprint();
	virtual void StopSprint();

	virtual void SwimForward(float Value) {};
	virtual void SwimRight(float Value) {};
	virtual void SwimUp(float Value) {};

	virtual void Mantle();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UGB_BaseCharacterMovementComp* GetBaseCharacterMovementComp() { return GB_BaseCharacterMovementComp; };

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintStart();
	virtual void OnSprintStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintEnd();
	virtual void OnSprintEnd_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	float SprintSpeed = 1200.0f;

	virtual bool CanSprint();

	UGB_BaseCharacterMovementComp* GB_BaseCharacterMovementComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	class ULedgeDetectorComponent* LedgeDetectorComponent;

private:
	void TryChangeSprintState();

	bool bIsSprintRequested = false;

};
