// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableItem.h"


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
