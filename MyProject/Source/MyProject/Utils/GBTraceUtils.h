#pragma once

namespace GBTraceUtils
{
	bool SweepCapsuleSingleByChannel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start, const FVector& End, float CapsuleRadius, float CapsuleHalfHeight, const FQuat& Rot, ECollisionChannel TraceChannel, const FCollisionQueryParams & Params = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& ResponseParams = FCollisionResponseParams::DefaultResponseParam, bool bDrawDebug = false, float DrawTime = -1.0f, FColor TraceColor = FColor::Black, FColor HitColor = FColor::Red);

	bool SweepSphereSingleByChannel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start, const FVector& End, float Radius, ECollisionChannel TraceChannel, const FCollisionQueryParams & Params = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& ResponseParams = FCollisionResponseParams::DefaultResponseParam, bool bDrawDebug = false, float DrawTime = -1.0f, FColor TraceColor = FColor::Black, FColor HitColor = FColor::Red);

	bool OverlapCapsuleAnyByProfile( const UWorld* World, const FVector& Pos, float CapsuleRadius, float CapsuleHalfHeight, FQuat Rot, FName ProfileName, const FCollisionQueryParams& QueryParamsm, bool bDrawDebug = false, float DrawTime = -1.0f, FColor HitColor = FColor::Green);

	bool OverlapCapsuleBlockingByProfile( const UWorld* World, const FVector& Pos, float CapsuleRadius, float CapsuleHalfHeight, FQuat Rot, FName ProfileName, const FCollisionQueryParams& QueryParamsm, bool bDrawDebug = false, float DrawTime = -1.0f, FColor HitColor = FColor::Green);
}