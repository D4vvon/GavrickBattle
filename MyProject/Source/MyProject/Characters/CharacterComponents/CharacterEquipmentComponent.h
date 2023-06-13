// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../GavrickBattleTypes.h"
#include "CharacterEquipmentComponent.generated.h"

typedef TArray<int32, TInlineAllocator<(uint32)EAmmunitionType::MAX>> TAmmunitionArray;
typedef TArray<class AEquipableItem*, TInlineAllocator<(uint32)EEquipmentSlots::MAX>> TItemsArray;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentWeaponAmmoChangedEvent, int32, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquippedItemChanged, const AEquipableItem*);

class AThrowableItem;
class ARangeWeaponItem;
class AMeleeWeaponItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	EEquipableItemType GetCurrentEquippedItemType() const;

	ARangeWeaponItem* GetCurrentRangeWeapon() const;

	AMeleeWeaponItem* GetCurrentMeleeWeapon() const;

	FOnCurrentWeaponAmmoChangedEvent OnCurrentWeaponAmmoChangedEvent;
	FOnEquippedItemChanged OnEquippedItemChanged;

	void ReloadCurrentWeapon();

	void EquipItemInSlot(EEquipmentSlots Slot);

	void AttachCurrentItemToEquippedSocket();

	void UnequipCurrentItem();

	void EquipPrimaryWeaponItem();
	void EquipSidearmWeaponItem();
	void EquipMeleeWeapon();

	bool IsEquipping();

	void EquipCurrentThrowableItem();
	void LaunchCurrentThrowableItem();

protected:
	
	virtual void BeginPlay();

	UFUNCTION()
	void OnCurrentWeaponAmmoChanged(int32 Ammo);

	EEquipmentSlots CurrentEquippedSlot;
	AEquipableItem* CurrentEquippedItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EAmmunitionType, int32> MaxAmmunitionAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EEquipmentSlots, TSubclassOf<class AEquipableItem>> ItemsLoadout;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TSet<EEquipmentSlots> IgnoreSlotsWhileSwitshing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loadout")
	EEquipmentSlots AutoEquipItemInSlot = EEquipmentSlots::None;

private:
	int32 GetAvailableAmmunitionForCurrentWeapon();

	bool bIsEquipping = false;
	FTimerHandle EquipTimer;

	void AutoEquip();

	UFUNCTION()
	void OnWeaponReloadComplete();

	void CreateLoadout();

	void EquipAnimationFinished();

	uint32 NextItemsArraySlotIndex(uint32 CurrentSlotIndex);
	uint32 PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex);

	ARangeWeaponItem* CurrentEquippedWeapon;
	TWeakObjectPtr<class AGB_BaseCharacter> CachedBaseCharacter;

	TAmmunitionArray AmmunitionArray;
	TItemsArray ItemsArray;

	FDelegateHandle OnCurrentWeaponAmmoChangedHandle;
	FDelegateHandle OnCurrentWeaponReloadHandle;

	EEquipmentSlots PreviousEquippedSlot;
	AThrowableItem* CurrentThrowableItem;

	AMeleeWeaponItem* CurrentMeleeWeapon;
};
