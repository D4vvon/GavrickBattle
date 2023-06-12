// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EquipableItem.h"
#include "RangeWeaponItem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnReloadComplete);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32);

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	Single,
	FullAuto
};

 class UAnimMontage;
UCLASS(Blueprintable)
class MYPROJECT_API ARangeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	ARangeWeaponItem();

	void StartFire();
	void StopFire();

	void StartAim();
	void StopAim();

	FTransform GetForegripTransform() const;

	float GetAimFOV();
	float GetAimMovementMaxSpeed();

	int32 GetAmmo() const;
	int32 GetMaxAmmo() const;
	void SetAmmo(int32 NewAmmo);

	bool CanShoot();

	void StartReload();
	void EndReload(bool bIsSuccess);

	EAmmunitionType GetAmmoType() const;

	FOnAmmoChanged OnAmmoChanged;

	FOnReloadComplete OnReloadComplete;

	virtual EReticleType GetReticleType() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWeaponBarrel* WeaponBarell;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
	UAnimMontage* WeaponFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Params", meta = (UIMin = 1.0f, ClampMin = 1.0f))
	float RateOfFire = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Params")
	EWeaponFireMode WeaponFireMode = EWeaponFireMode::Single;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Params", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 2.0f, ClampMax = 2.0f))
	float SpreadAngle = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Params", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 1.0f, ClampMax = 1.0f))
	float AimSpreadAngle = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Params")
	float AimMovementMaxSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Params")
	float AimFOV = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Character")
	UAnimMontage* CharacterFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Params", meta = (UIMin = 1.0f, ClampMin = 1.0f))
	int32 MaxAmmo = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Params")
	EAmmunitionType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Params")
	bool bAutoReload = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
	UAnimMontage* WeaponReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Character")
	UAnimMontage* CharacterReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reticle")
	EReticleType AimReticleType = EReticleType::None;

private:
	int32 Ammo = 0;

	bool bIsReloading = false;
	
	float GetCurrentBulletSpread();

	void MakeShot();

	float GetShotTimerInterval();

	float PlayAnimMontage(UAnimMontage* AnimMontage);
	void StopAnimMontage(UAnimMontage* AnimMontage, float BlendOutTime = 0.0f);

	FTimerHandle ShotTimer;
	FTimerHandle ReloadTimer;

	bool bIsAiming = false;

	bool bIsFiring = false;

	void OnShotTimerElapsed();
};
