// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponBarrel.generated.h"

USTRUCT(BlueprintType)
struct FDecalInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
	FVector DecalSize = FVector(5.0f, 5.0f, 5.0f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
	float DecalLifetime = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
	float DecalFadeoutTime = 5.0f;
};

UENUM(BlueprintType)
enum class EHitRegistrationType : uint8
{
	HitScan,
	Projectile
};

class UNiagaraSystem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UWeaponBarrel : public USceneComponent
{
	GENERATED_BODY()

public:	
	void Shot(FVector ShotStart, FVector ShotDirection, float SpreadAngle);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes")
	float FiringRange = 5000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes")
	float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	int32 BulletsPerShot = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes")
	UNiagaraSystem* MuzzleFlashFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes")
	UParticleSystem* MuzzleFlashFX_Paricle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes")
	FVector MuzzleOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Decals")
	FDecalInfo DefaultDecalInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes")
	TSubclassOf<class UDamageType> DamgeTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Hit registration")
	EHitRegistrationType HitRegistration = EHitRegistrationType::HitScan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Hit registration", meta = (EditCondition = "HitRegistration == EHitRegistrationType::Projectile"))
	TSubclassOf<class AGB_Projectile> ProjectileClass;

private:
	APawn* GetOwningPawn();
	AController* GetController();

	UFUNCTION()
	void ProcessHit(const FHitResult& HitResilt, const FVector& Direction);

	bool HitScan(FVector ShotStart, OUT  FVector& ShotEnd, FVector ShotDirection);

	void LaunchProjectile(FVector ShotStart, FVector ShotDirection, FVector ShotEnd);
	bool bIsTraceEnable = false;

	FVector GetBulletSpreadOffset(float Angle, FRotator ShotRotation);
		
};
