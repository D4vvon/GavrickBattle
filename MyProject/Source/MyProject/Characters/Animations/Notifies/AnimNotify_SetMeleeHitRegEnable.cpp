// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SetMeleeHitRegEnable.h"
#include "../../GB_BaseCharacter.h"
#include "../../CharacterComponents/CharacterEquipmentComponent.h"
#include "../../../Actor/Equipment/Weapons/MeleeWeaponItem.h"

void UAnimNotify_SetMeleeHitRegEnable::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AGB_BaseCharacter* CharacterOwner = Cast<AGB_BaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	AMeleeWeaponItem* MeleeWeapon = CharacterOwner->GetCharacterEquipmentComponent()->GetCurrentMeleeWeapon();
	if (IsValid(MeleeWeapon))
	{
		MeleeWeapon->SetIsHitRegistrationEnabled(bIsHitRegistrationEnabled);
	}
}
