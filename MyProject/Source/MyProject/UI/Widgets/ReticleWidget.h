// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../GavrickBattleTypes.h"
#include "ReticleWidget.generated.h"

class AEquipableItem;

UCLASS()
class MYPROJECT_API UReticleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnAimingStateChange(bool bIsAiming);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reticle")
	EReticleType CurrentReticle = EReticleType::None;

	UFUNCTION(BlueprintNativeEvent)
	void OnEquippedItemChanged(const AEquipableItem* EquippedItem);

private:
	TWeakObjectPtr<const AEquipableItem> CurrentEquippedItem;

	void SetupCurrentReticle();
};
