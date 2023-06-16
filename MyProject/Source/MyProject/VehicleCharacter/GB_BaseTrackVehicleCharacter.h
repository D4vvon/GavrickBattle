// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GB_BaseTrackVehicleCharacter.generated.h"

class UBoxComponent;
UCLASS(Abstract, NotBlueprintable)
class MYPROJECT_API AGB_BaseTrackVehicleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGB_BaseTrackVehicleCharacter();

	virtual void BeginPlay() override;

	virtual void StartMoveForward(float Value) {};
	virtual void StartMoveRight(float Value) {};

	virtual void StartRotateTurret(float Value) {};
	virtual void StartMoveGun(float Value) {};

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionComponents")
	//UBoxComponent* BodyCollision;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionComponents")
	//UBoxComponent* TurretCollision;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionComponents")
	//UBoxComponent* MuzzleCollision;

};
