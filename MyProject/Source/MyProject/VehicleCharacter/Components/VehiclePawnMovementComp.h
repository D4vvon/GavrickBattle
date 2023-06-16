// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "VehiclePawnMovementComp.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UVehiclePawnMovementComp : public UPawnMovementComponent
{
	GENERATED_BODY()

public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float MaxSpeed = 1500.0f;

private:
	FVector VerticalVelocity = FVector::ZeroVector;

	bool bIsFalling = false;
};
