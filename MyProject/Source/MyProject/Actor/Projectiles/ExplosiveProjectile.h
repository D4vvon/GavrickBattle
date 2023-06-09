// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB_Projectile.h"
#include "ExplosiveProjectile.generated.h"

/**
 * 
 */
 class UExplosiomComponent;
UCLASS()
class MYPROJECT_API AExplosiveProjectile : public AGB_Projectile
{
	GENERATED_BODY()

public:
	AExplosiveProjectile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UExplosiomComponent* ExplosionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion")
	float DetonationTime = 2.0f;

	virtual void OnProjectileLaunched() override;

	AController* GetController();

private:
	void OnDetonationTimerElapsed();

	FTimerHandle DetonationTimer;
	
};
