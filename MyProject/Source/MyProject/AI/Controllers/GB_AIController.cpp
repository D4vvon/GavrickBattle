// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_AIController.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

AGB_AIController::AGB_AIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

AActor* AGB_AIController::GetClosestSensedActor(TSubclassOf<UAISense> SenseClass)
{
	if (!IsValid(GetPawn()))
	{
		return nullptr;
	}

	TArray<AActor*> SensedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(SenseClass, SensedActors);

	AActor* CloseltActor = nullptr;
	float MinSquaredDistance = FLT_MAX;
	FVector PawnLocation = GetPawn()->GetActorLocation();

	for (AActor* SeenActor : SensedActors)
	{
		float CurrentSquaredDistance = (PawnLocation - SeenActor->GetActorLocation()).SizeSquared();
		if (CurrentSquaredDistance < MinSquaredDistance)
		{
			MinSquaredDistance = CurrentSquaredDistance;
			CloseltActor = SeenActor;
		}
	}
	return CloseltActor;
}
