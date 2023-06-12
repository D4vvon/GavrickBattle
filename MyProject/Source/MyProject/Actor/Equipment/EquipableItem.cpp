// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableItem.h"
#include "../../Characters/GB_BaseCharacter.h"
#include "GameFramework/Actor.h"


void AEquipableItem::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	if (IsValid(NewOwner))
	{
		CachedCharacterOwner = StaticCast<AGB_BaseCharacter*>(GetOwner());
	}
	else
	{
		CachedCharacterOwner = nullptr;
	}
}

UAnimMontage* AEquipableItem::GetCharacterEquipAnimMontage()
{
	return CharacterEquipAnimMontage;
}

EEquipableItemType AEquipableItem::GetItemType() const
{
	return ItemType;
}

FName AEquipableItem::GetUnEquippedSocketName() const
{
	return UnequippedSocketName;
}

FName AEquipableItem::GetEquippedSocketName() const
{
	return EquippedSocketName;
}

void AEquipableItem::Equip()
{
	if (OnEquipmentStateChange.IsBound())
	{
		OnEquipmentStateChange.Broadcast(true);
	}
}

void AEquipableItem::UnEquip()
{
	if (OnEquipmentStateChange.IsBound())
	{
		OnEquipmentStateChange.Broadcast(false);
	}
}

EReticleType AEquipableItem::GetReticleType() const
{
	return ReticleType;
}

AGB_BaseCharacter* AEquipableItem::GetCharacterOwner() const
{
	return CachedCharacterOwner.IsValid() ? CachedCharacterOwner.Get() : nullptr;
}
