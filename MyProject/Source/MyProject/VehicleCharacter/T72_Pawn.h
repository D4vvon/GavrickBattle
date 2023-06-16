// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "T72_Pawn.generated.h"

class UCameraComponent;
class UBoxComponent;
UCLASS()
class MYPROJECT_API AT72_Pawn : public APawn
{
	GENERATED_BODY()

public:
	AT72_Pawn();

	virtual void Tick(float DeltaSeconds);

	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartMoveForward(float Value);
	void StartMoveRight(float Value);

	void StartRotateTurret(float Value);
	void StartMoveMuzzle(float Value);

	//Getters for animInstance
	float GetTurretRotationSpeed();
	float GetMuzzleRotationSpeed();

	bool GetIsTurretRotate();
	bool GetIsMuzzleRotate();
	//bool GetIsRotationTurretWhileRotateBody();

	void SetTurretDirection(float NewDirection);
	float GetTurretDiretion();

	void SetMuzzleDirection(float NewDirection);
	float GetMuzzleDirection();
	//~GettersForAnimInstnce

	void StartAiming();
	void StopAiming();

	bool GetIsAiming();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere)
	float BodyRotationSpeed = 20.0f;

	UPROPERTY(EditAnywhere)
	float TurretRotationSpeed = 20.0f;

	UPROPERTY(EditAnywhere)
	float MuzzleRotationSpeed = 15.0f;
	UPROPERTY(EditAnywhere)
	float MuzzleMaxAngle = 0.07f; //radians
	UPROPERTY(EditAnywhere)
	float MuzzleMinAngle = -0.1f; //radians

	float CurrentMuzzleAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CollisionComps")
	UBoxComponent* BodyCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CollisionComps")
	UBoxComponent* TurretCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CollisionComps")
	UBoxComponent* MuzzleCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraComps")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraComps")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraComps")
	UCameraComponent* AimingCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CollisionComps")
	class UT72_MovementComponent* MovementComponent;

private:
	void UpdateOrientation(float DeltaTime);
	void UpdateVelocityAndPosition(float DelatTime);
	
	FVector VerticalVelocity = FVector::ZeroVector;

	bool bIsMoving = false;
	int32 bIsMoveForward = 1;

	bool bIsAiming = false;

	FVector RotationBodyInput = FVector::ZeroVector;
	FVector RotationTurretInput = FVector::ZeroVector;
	FVector RotationMuzzleInput = FVector::ZeroVector;

	bool bIsRotatingBody = false;
	bool bIsRotatingTurret = false;
	bool bIsRotationTurretWhileRotateBody = true;
	bool bIsRotatingMuzzle = false;
	
	float TurretDirection = 1.0f;
	float MuzzleDirection = 1.0f;

};
