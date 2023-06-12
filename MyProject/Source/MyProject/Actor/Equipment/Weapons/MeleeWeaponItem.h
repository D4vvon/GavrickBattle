// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EquipableItem.h"
#include "../../../GavrickBattleTypes.h"
#include "MeleeWeaponItem.generated.h"

USTRUCT(BlueprintType)
struct FMeleeAttackDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Attack")
	TSubclassOf<class UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Attack", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float DamageAmount = 42.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Attack")
	class UAnimMontage* AttackMontage;
};

class UMeleeHitRegistrator;
UCLASS(Blueprintable)
class MYPROJECT_API AMeleeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	AMeleeWeaponItem();

	void StartAttack(EMeleeAttackTypes AttackType);

	void SetIsHitRegistrationEnabled(bool bIsRegistrationEnabled);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MeleeAttack")
	TMap<EMeleeAttackTypes, FMeleeAttackDescription> Attacks;

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ProcessHit(const FHitResult& HitResult, const FVector& HitDirection);

	TArray<UMeleeHitRegistrator*> HitRegistrators;
	TSet<AActor*> HitActors;

	void OnAttackTimerElapsed();
	FTimerHandle AttackTimer;

	FMeleeAttackDescription* CurrentAttack;
};
