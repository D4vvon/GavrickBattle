// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../GavrickBattleTypes.h"
#include "EquipableItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentStateChange, bool, bIsEquipped);

class AGB_BaseCharacter;
class UAnimMontage;
UCLASS(Abstract, NotBlueprintable)
class MYPROJECT_API AEquipableItem : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void SetOwner( AActor* NewOwner ) override;

	UAnimMontage* GetCharacterEquipAnimMontage();

	EEquipableItemType GetItemType() const;

	FName GetUnEquippedSocketName() const;
	FName GetEquippedSocketName() const;

	virtual void Equip();
	virtual void UnEquip();

	virtual EReticleType GetReticleType() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipableItem")
	EEquipableItemType ItemType = EEquipableItemType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipableItem")
	FName UnequippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipableItem")
	FName EquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipableItem")
	UAnimMontage* CharacterEquipAnimMontage;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentStateChange OnEquipmentStateChange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reticle")
	EReticleType ReticleType = EReticleType::None;

	AGB_BaseCharacter* GetCharacterOwner() const;

private:
	TWeakObjectPtr<AGB_BaseCharacter> CachedCharacterOwner;
};
