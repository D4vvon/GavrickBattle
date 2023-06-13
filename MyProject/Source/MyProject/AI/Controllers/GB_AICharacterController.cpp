// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_AICharacterController.h"
#include "../Characters/GB_AICharacter.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/AIModule/Classes/Perception/AISense_Sight.h"
#include "../../Characters/CharacterComponents/AIPatrollingComponent.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "../../GavrickBattleTypes.h"

void AGB_AICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		CachedAICharacter = StaticCast<AGB_AICharacter*>(InPawn);
		RunBehaviorTree(CachedAICharacter->GetBehaviorTree());
	}
	else
	{
		CachedAICharacter = nullptr;
	}
}

void AGB_AICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
	if (!CachedAICharacter.IsValid())
	{
		return;
	}
	TryMoveToNextTarget();
}

void AGB_AICharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	if (!Result.IsSuccess())
	{
		return;
	}
	TryMoveToNextTarget();
}

void AGB_AICharacterController::BeginPlay()
{
	Super::BeginPlay();
	UAIPatrollingComponent* PatrollingComponent = CachedAICharacter->GetPatrollingComponent();
	if (PatrollingComponent->CanPatroll())
	{
		FVector ClosestWayPoint = PatrollingComponent->SelectClosestWayPoint();
		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsVector(BB_NextLocation, ClosestWayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}
		bIsPatrolling = true;
	}

}

void AGB_AICharacterController::TryMoveToNextTarget()
{
	UAIPatrollingComponent* PatrollingComponent = CachedAICharacter->GetPatrollingComponent();
	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	if (IsValid(ClosestActor))
	{
		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsObject(BB_CurrentTarget, ClosestActor);
		}
		bIsPatrolling = false;
	}
	else if(PatrollingComponent->CanPatroll())
	{
		FVector WayPoint = bIsPatrolling ? PatrollingComponent->SelectNexWayPoint() : PatrollingComponent->SelectClosestWayPoint();
		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsVector(BB_NextLocation, WayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}
		bIsPatrolling = true;
	}
}

bool AGB_AICharacterController::IsTargetReached(FVector TargetLocation) const
{
	return (TargetLocation - CachedAICharacter->GetActorLocation()).SizeSquared() <= FMath::Square(TargetReachRadius);
}
