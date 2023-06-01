// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "../../Characters/GB_BaseCharacter.h"
#include "../../Characters/CharacterComponents/CharacterAttributesComponent.h"
#include "ReticleWidget.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/UMG/Public/Blueprint/WidgetTree.h"
#include "AmmoWidget.h"

UReticleWidget* UPlayerHUDWidget::GetReticleWidget()
{
	return WidgetTree->FindWidget<UReticleWidget>(ReticleWidgetName);
}

class UAmmoWidget* UPlayerHUDWidget::GetAmmoWidget()
{
	return WidgetTree->FindWidget<UAmmoWidget>(AmmoWidgetName);
}

float UPlayerHUDWidget::GetHealthPercent() const
{
	float Result = 1.0f;
	APawn* Pawn = GetOwningPlayerPawn();
	AGB_BaseCharacter* Character = Cast<AGB_BaseCharacter>(Pawn);
	if (IsValid(Character))
	{
		const UCharacterAttributesComponent* CharacterAttributes = Character->GetCharacterAttributeConponent();
		Result = CharacterAttributes->GetHealthPercent();
	}
	return Result;
}
