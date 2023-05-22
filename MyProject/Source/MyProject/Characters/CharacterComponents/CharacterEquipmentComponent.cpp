// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEquipmentComponent.h"
#include "../GB_BaseCharacter.h"
#include "../../Actor/Equipment/Weapons/RangeWeaponItem.h"
#include "../../GavrickBattleTypes.h"

EEquipableItemType UCharacterEquipmentComponent::GetCurrentEquippedItemType() const
{
	EEquipableItemType Result = EEquipableItemType::None;
	if (IsValid(CurrentEquippedWeapon))
	{
		Result = CurrentEquippedWeapon->GetItemType();
	}
	return Result;
}

void UCharacterEquipmentComponent::Fire()
{
	if (IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->Fire();
	}
}

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<AGB_BaseCharacter>(), TEXT("UCharacterEquipmentComponent::BeginPlay() onlu with AGB_BaseCharacter"))
	CachedBaseCharacter = StaticCast<AGB_BaseCharacter*>(GetOwner());
	CreateLoadout();
	
}

void UCharacterEquipmentComponent::CreateLoadout()
{
	if (!IsValid(SideArmClass))
	{
		return;
	}
	CurrentEquippedWeapon = GetWorld()->SpawnActor<ARangeWeaponItem>(SideArmClass);
	CurrentEquippedWeapon->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketCharacterWeapon);
	CurrentEquippedWeapon->SetOwner(CachedBaseCharacter.Get());
}

