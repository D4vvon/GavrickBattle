// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GB_BasePlanePawn.generated.h"

UCLASS()
class MYPROJECT_API AGB_BasePlanePawn : public APawn
{
	GENERATED_BODY()

public:
	AGB_BasePlanePawn();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddYaw(float Value);
	void AddPitch(float Value);
	void AddRoll(float Value);

	void Accelerate(float Value);
	void Decelarate(float Value);

	class UBoxComponent* GetMoverComp();

	class USkeletalMeshComponent* GetSkelMesh();

	class USu33MovementComponent* GetSu33Movement();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* MoverComponent;
	
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere)
	UPawnMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* FrontWheel;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* RightWheel;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* LeftWheel;

private:
	float PlayAnimMontage(class UAnimMontage* AnimMontage);
};
