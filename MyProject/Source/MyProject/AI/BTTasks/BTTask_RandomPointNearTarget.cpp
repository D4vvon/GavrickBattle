// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RandomPointNearTarget.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/AIModule/Classes/AIController.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/NavigationSystem/Public/NavigationSystem.h"

UBTTask_RandomPointNearTarget::UBTTask_RandomPointNearTarget()
{
	NodeName = "Random point near target";
}

EBTNodeResult::Type UBTTask_RandomPointNearTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid( AIController) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!IsValid(Pawn))
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Pawn);
	if (!IsValid(NavSystem))
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!IsValid(TargetActor))
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation NavLocation;

	bool bIsFound = NavSystem->GetRandomReachablePointInRadius(TargetActor->GetActorLocation(), Radius, NavLocation);
	if (!bIsFound)
	{
		return EBTNodeResult::Failed;
	}

	Blackboard->SetValueAsVector(LocationKey.SelectedKeyName, NavLocation.Location);
	return EBTNodeResult::Succeeded;
}
