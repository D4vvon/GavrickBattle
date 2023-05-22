// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GB_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AGB_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void SetPawn(APawn* InPawn) override;

protected:

	virtual void SetupInputComponent() override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void ChangeCrouchState();

	void StartSprint();
	void StopSprint();

	void SwimForward(float Value);
	void SwimRight(float Value);
	void SwimUp(float Value);

	void Mantle();

	void Jump();

	void ClimbLadderUp(float Value);

	void InteractWithLadder();

	void Fire();

	TSoftObjectPtr<class AGB_BaseCharacter> CachedBaseCharacter;
	
};
