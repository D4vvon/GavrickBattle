// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Su33MovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API USu33MovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void SetAcceleration(float Value);
	void SetDeceleration(float Value);

protected:
	UPROPERTY(EditAnywhere)
	FVector RotationSpeed = FVector(60.0f, 100.0f, 20.0f);

	UPROPERTY(EditAnywhere)
	float NormalSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 5000.0f;

	UPROPERTY(EditAnywhere)
	float StallThreshold = 500.0f;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 10.0f;
	UPROPERTY(EditAnywhere)
	float AirInterpSpeed = 200.0f;

	UPROPERTY(EditAnywhere)
	float FallingInterpSpeed = 10.0f;

	bool bIsInAir = true;

	class USkeletalMeshComponent* SkelMesh = nullptr;
/* Curves
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCurveFloat* AccelerationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCurveFloat* DecelerationCurve;*/

	virtual void BeginPlay() override;

	virtual float GetMaxSpeed();

	virtual bool MoveUpdatedComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit /* = NULL */, ETeleportType Teleport /* = ETeleportType::None */) override;

private:
	FQuat GetNewRotation(float DeltaTime);
	void PerformMovement(float DeltaTime);
	void UpdateVelocity(float DeltaTime);

	class AGB_BasePlanePawn* GetPlaneOwner();
	
	float CurrentSpeed = 0.0f;
	float TargetSpeed = 0.0f;
	float FallingSpeed = 0.0f;


	float Accelaration = 0.0f;
	float Decelaration = 0.0f;
};
