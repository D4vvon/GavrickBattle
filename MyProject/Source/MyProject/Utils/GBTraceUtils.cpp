#include "GBTraceUtils.h"
#include "DrawDebugHelpers.h"

bool GBTraceUtils::SweepCapsuleSingleByChannel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start, const FVector& End, float CapsuleRadius, float CapsuleHalfHeight, const FQuat& Rot, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params, const FCollisionResponseParams& ResponseParams, bool bDrawDebug, float DrawTime, FColor TraceColor, FColor HitColor)
{
	bool bResult = false;

	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	bResult = World->SweepSingleByChannel(OutHit, Start, End, Rot, TraceChannel, CollisionShape, Params, ResponseParams);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		DrawDebugCapsule(World, Start, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, TraceColor, false, DrawTime);
		DrawDebugCapsule(World, End, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, TraceColor, false, DrawTime);
		DrawDebugLine(World, Start, End, TraceColor, false, 2.0f);

		if (bResult)
		{
			DrawDebugCapsule(World, OutHit.Location, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, HitColor, false, DrawTime);
			DrawDebugPoint(World, OutHit.ImpactPoint, 20.0f, FColor::Red, false, DrawTime);
		}
	}
#endif

	return bResult;
}

bool GBTraceUtils::SweepSphereSingleByChannel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start, const FVector& End, float Radius, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params /*= FCollisionQueryParams::DefaultQueryParam*/, const FCollisionResponseParams& ResponseParams /*= FCollisionResponseParams::DefaultResponseParam*/, bool bDrawDebug /*= false*/, float DrawTime /*= -1.0f*/, FColor TraceColor /*= FColor::Black*/, FColor HitColor /*= FColor::Red*/)
{
	bool bResult = false;

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
	bResult = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, TraceChannel, CollisionShape, Params, ResponseParams);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{

		FVector DebugCenter = (Start + End) * 0.5;
		FVector TraceVector = End - Start;
		float DegubDrawCapsuleHalfHeight = TraceVector.Size() * 0.5f;

		FQuat DebugCapsuleRotation = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();

		DrawDebugCapsule(World, DebugCenter, DegubDrawCapsuleHalfHeight, Radius, DebugCapsuleRotation, TraceColor, false, DrawTime);

		if (bDrawDebug)
		{
			DrawDebugSphere(World, OutHit.Location, Radius, 32, HitColor, false, DrawTime);
			DrawDebugPoint(World, OutHit.ImpactPoint, 20.0f, HitColor, false, DrawTime);
		}
	}
#endif

	return bResult;
}

bool GBTraceUtils::OverlapCapsuleAnyByProfile(const UWorld* World, const FVector& Pos, float CapsuleRadius, float CapsuleHalfHeight, FQuat Rot, FName ProfileName, const FCollisionQueryParams& QueryParamsm, bool bDrawDebug /*= false*/, float DrawTime /*= -1.0f*/, FColor HitColor /*= FColor::Red*/)
{
	bool bResult = false;

	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	bResult = World->OverlapAnyTestByProfile(Pos, Rot, ProfileName, CollisionShape, QueryParamsm);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug && bResult)
	{
		DrawDebugCapsule(World, Pos, CapsuleHalfHeight, CapsuleRadius, Rot, HitColor, false, DrawTime);
	}
#endif

	return bResult;
}
