// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Fire.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/AIModule/Classes/AIController.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "../../Characters/GB_BaseCharacter.h"
#include "../../Actor/Equipment/Weapons/RangeWeaponItem.h"
#include "../../Characters/CharacterComponents/CharacterEquipmentComponent.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

UBTService_Fire::UBTService_Fire()
{
	NodeName = "Fire";
}

void UBTService_Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(BlackboardComponent))
	{
		return;
	}
	
	AGB_BaseCharacter* Character = Cast<AGB_BaseCharacter>(AIController->GetPawn());
	if (!IsValid(Character))
	{
		return;
	}

	UCharacterEquipmentComponent* EquipmentComponent = Character->GetCharacterEquipmentComponent_Mutable();
	ARangeWeaponItem* RangeeWeapon = EquipmentComponent->GetCurrentRangeWeapon();

	if (!IsValid(RangeeWeapon))
	{
		return;
	}

	AActor* CurrentTarget = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!IsValid(CurrentTarget))
	{
		Character->StopFire();
		return;
	}

	float DistSq = FVector::DistSquared(CurrentTarget->GetActorLocation(), Character->GetActorLocation());
	if (DistSq > FMath::Square(MaxFireDistanse))
	{
		Character->StopFire();
		return;
	}

	if (!RangeeWeapon->IsRelaoding() || !RangeeWeapon->IsFiring())
	{
		Character->StartFire();
	}
}
