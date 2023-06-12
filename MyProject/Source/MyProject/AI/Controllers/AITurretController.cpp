// Fill out your copyright notice in the Description page of Project Settings.


#include "AITurretController.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/AIModule/Classes/Perception/AISense_Sight.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "../Characters/Turret.h"

void AAITurretController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		CachedTurret = StaticCast<ATurret*>(InPawn);
	}
	else
	{
		CachedTurret = nullptr;
	}
}

void AAITurretController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
	if (!CachedTurret.IsValid())
	{
		return;
	}

	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	CachedTurret->SetCurrentTarget(ClosestActor);
}
