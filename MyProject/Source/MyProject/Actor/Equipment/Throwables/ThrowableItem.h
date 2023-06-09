// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EquipableItem.h"
#include "ThrowableItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYPROJECT_API AThrowableItem : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	void Throw();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwables")
	TSubclassOf<class AGB_Projectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throwables", meta = (UIMin = -90.f, UIMax = 90.0f, ClampMin = -90.0f, ClampMax = 90.0f))
	float ThrowAngle = 0.0f;
};
