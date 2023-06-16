// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "T72_Controller.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AT72_Controller : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetPawn(APawn* InPawn) override;

protected:
	virtual void SetupInputComponent() override;

private:
	TWeakObjectPtr<class AT72_Pawn> CachedT72_Pawn;

	void StartMoveForward(float Value);
	void StartMoveRight(float Value);

	void StartRotateTurret(float Value);
	void StartMoveMuzzle(float Value);

	void StartAiming();
	void StopAiming();
};
