// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPatrollingComponent.h"
#include "../../Actor/Navigation/PatrollingPath.h"


bool UAIPatrollingComponent::CanPatroll() const
{
	return IsValid(PatrollingPath) && PatrollingPath->GetWayPoints().Num() > 0;
}

FVector UAIPatrollingComponent::SelectClosestWayPoint()
{
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	const TArray<FVector> WayPoints = PatrollingPath->GetWayPoints();
	FTransform PathTransform = PatrollingPath->GetActorTransform();

	FVector ClosestWayPoint;

	float MinSquaredDistance = FLT_MAX;
	for (int32 i = 0; i < WayPoints.Num(); ++i)
	{
		FVector WayPointWorld = PathTransform.TransformPosition(WayPoints[i]);
		float CurrentSqDistance = (OwnerLocation - WayPointWorld).SizeSquared();
		if (CurrentSqDistance < MinSquaredDistance)
		{
			MinSquaredDistance = CurrentSqDistance;
			ClosestWayPoint = WayPointWorld;
			CurrentWayPointIndex = i;
		}
	}
	return ClosestWayPoint;
}

FVector UAIPatrollingComponent::SelectNexWayPoint()
{
	++CurrentWayPointIndex;
	const TArray<FVector> WayPoints = PatrollingPath->GetWayPoints();
	if (CurrentWayPointIndex == PatrollingPath->GetWayPoints().Num())
	{
		CurrentWayPointIndex = 0;
	}
	FTransform PathTransform = PatrollingPath->GetActorTransform();
	FVector WayPoint = PathTransform.TransformPosition(WayPoints[CurrentWayPointIndex]);
	return WayPoint;
}

