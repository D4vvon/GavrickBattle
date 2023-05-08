// Fill out your copyright notice in the Description page of Project Settings.


#include "LedgeDetectorComponent.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "../../GavrickBattleTypes.h"
#include "DrawDebugHelpers.h"
#include "../../Utils/GBTraceUtils.h"


void ULedgeDetectorComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<ACharacter>(), TEXT("ULedgeDetectorComponent::BeginPlay() only ACharacter can works with ULedgeDetector"));
	CachedCharacterOwner = StaticCast<ACharacter*>(GetOwner());
}

bool ULedgeDetectorComponent::DetectLedge(OUT FLedgeDescription& LedgeDescription)
{
	UCapsuleComponent* CapsuleComponent = CachedCharacterOwner->GetCapsuleComponent();

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());

	float BottomZOffset = 2.0f;
	FVector CharacterBottom = CachedCharacterOwner->GetActorLocation() - (CapsuleComponent->GetScaledCapsuleHalfHeight() - BottomZOffset) * FVector::UpVector;

	float ForwardCheckCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float ForwardCheckCapsuleHalfHeight = (MaximumLedgeHeight - MinimumLedgeHeight) * 0.5;

	FHitResult ForwardCheckHitResult;
	FVector ForwardStartLocation = CharacterBottom + (MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	FVector ForwardEndLocation = ForwardStartLocation + CachedCharacterOwner->GetActorForwardVector() * ForwardCheckDistance;

	float DrawTime = 2.0f;

	if (!GBTraceUtils::SweepCapsuleSingleByChannel(GetWorld(), ForwardCheckHitResult, ForwardStartLocation, ForwardEndLocation, ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight, FQuat::Identity, ECC_Climbing, QueryParams, FCollisionResponseParams::DefaultResponseParam, true, DrawTime))
	{
		return false;
	}

	FHitResult DownwardCheckHitResult;
	float DownwardShpereCheckRadius = CapsuleComponent->GetScaledCapsuleRadius();

	float DownwardCheckDepthOffset = 10.0f;
	FVector DownwardTraceStartLocation = ForwardCheckHitResult.ImpactPoint + ForwardCheckHitResult.ImpactNormal * DownwardCheckDepthOffset;
	DownwardTraceStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardShpereCheckRadius;
	FVector DownwardTraceEndLocation(DownwardTraceStartLocation.X, DownwardTraceStartLocation.Y, CharacterBottom.Z);

	FVector DebugDrawCapsuleLocation = (DownwardTraceStartLocation + DownwardTraceEndLocation) * 0.5;
	float DegubDrawCapsuleHalfHeight = (DownwardTraceEndLocation - DownwardTraceStartLocation).Size() * 0.5f;

	if (!GBTraceUtils::SweepSphereSingleByChannel(GetWorld(), DownwardCheckHitResult, DownwardTraceStartLocation, DownwardTraceEndLocation, DownwardShpereCheckRadius, ECC_Climbing, QueryParams, FCollisionResponseParams::DefaultResponseParam, true, DrawTime))
	{
		return false;
	}

	float OverlapCapsuleFloorOffset = 2.0f;
	FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + (CapsuleComponent->GetScaledCapsuleHalfHeight() + OverlapCapsuleFloorOffset) * FVector::UpVector;


	if (GBTraceUtils::OverlapCapsuleAnyByProfile(GetWorld(), OverlapLocation, CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight(), FQuat::Identity, FName("Pawn"), QueryParams, true, DrawTime))
	{
		return false;
	}

	LedgeDescription.Location = DownwardCheckHitResult.ImpactPoint;
	LedgeDescription.Rotation = (ForwardCheckHitResult.ImpactNormal * FVector(-1.0f, -1.0f, 0.0f)).ToOrientationRotator();

	return true;
}

