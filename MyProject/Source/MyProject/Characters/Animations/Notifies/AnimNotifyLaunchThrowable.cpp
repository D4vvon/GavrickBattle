// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyLaunchThrowable.h"
#include "../../GB_BaseCharacter.h"
#include "../../CharacterComponents/CharacterEquipmentComponent.h"

void UAnimNotifyLaunchThrowable::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AGB_BaseCharacter* CharacterOwner = Cast<AGB_BaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	CharacterOwner->GetCharacterEquipmentComponent_Mutable()->LaunchCurrentThrowableItem();
}
