// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../GavrickBattleTypes.h"
#include "CharacterEquipmentComponent.generated.h"

typedef TArray<int32, TInlineAllocator<(uint32)EAmmunitionType::MAX>> TAmmunitionArray;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentWeaponAmmoChangedEvent, int32, int32);

class ARangeWeaponItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	EEquipableItemType GetCurrentEquippedItemType() const;

	ARangeWeaponItem* GetCurrentRangeWeapon() const;

	FOnCurrentWeaponAmmoChangedEvent OnCurrentWeaponAmmoChangedEvent;

	void ReloadCurrentWeapon();

protected:
	
	virtual void BeginPlay();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TSubclassOf<ARangeWeaponItem> SideArmClass;

	UFUNCTION()
	void OnCurrentWeaponAmmoChanged(int32 Ammo);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EAmmunitionType, int32> MaxAmmunitionAmount;

private:
	int32 GetAvailableAmmunitionForCurrentWeapon();

	UFUNCTION()
	void OnWeaponReloadComplete();

	void CreateLoadout();

	ARangeWeaponItem* CurrentEquippedWeapon;
	TWeakObjectPtr<class AGB_BaseCharacter> CachedBaseCharacter;

	TAmmunitionArray AmmunitionArray;
};
