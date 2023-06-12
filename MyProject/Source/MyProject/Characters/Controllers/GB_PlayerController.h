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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;

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

	void StartingFire();
	void StopingFire();

	void StartAiming();
	void StopAiming();

	void Reload();

	void NextItem();
	void PreviousItem();
	void MeleeItem();

	void EquipPrimaryItem();

	void PrimaryMeleeAttack();
	void SecondaryMeleeAttack();

	float BaseTurnRate = 20.0f;
	float BaseLookUpRate = 20.0f;
	float AimTurnRate = 1.0f;
	float AimLookUpRate = 1.0f;

	TSoftObjectPtr<class AGB_BaseCharacter> CachedBaseCharacter;

	void CreateAndInitializeWidgets();
	UPlayerHUDWidget* PlayerHUDWidget = nullptr;
	
};
