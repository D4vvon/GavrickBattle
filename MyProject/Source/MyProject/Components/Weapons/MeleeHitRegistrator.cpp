// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeHitRegistrator.h"
#include "../../GavrickBattleTypes.h"
#include "../../SubSystems/DebugSubsystem.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../../Utils/GBTraceUtils.h"

UMeleeHitRegistrator::UMeleeHitRegistrator()
{
	PrimaryComponentTick.bCanEverTick = true;
	SphereRadius = 5.0f;
	SetCollisionProfileName(CollisionProfileNoCollision);
}

void UMeleeHitRegistrator::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsHitRegistrationEnabled)
	{
		ProcessHitRegistration();
	}
	PreviousComponentLocation = GetComponentLocation();
}

void UMeleeHitRegistrator::SetIsHitRegistrationEnable(bool bIsEnabled_In)
{
	bIsHitRegistrationEnabled = bIsEnabled_In;
}

void UMeleeHitRegistrator::ProcessHitRegistration()
{
	FVector CurrentLocation = GetComponentLocation();

	FHitResult HitResult;

#if ENABLE_DRAW_DEBUG
	UDebugSubsystem* DebugSubSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubSystem->IsCategoryEnabled(DebugCategoryMeleeWeapon);
#else
	bool bIsDebugEnabled = false;
#endif
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = true;
	CollisionQueryParams.AddIgnoredActor(GetOwner()->GetOwner());

	bool bHasHit = GBTraceUtils::SweepSphereSingleByChannel(
				GetWorld(),
				HitResult,
				PreviousComponentLocation,
				CurrentLocation,
				GetScaledSphereRadius(),
				ECC_Melee,
				CollisionQueryParams,
				FCollisionResponseParams::DefaultResponseParam,
				bIsDebugEnabled,
				5.0f
	);
	if (bHasHit)
	{
		FVector Direction = (CurrentLocation - PreviousComponentLocation).GetSafeNormal();
		if (OnMeleeHitRegistred.IsBound())
		{
			OnMeleeHitRegistred.Broadcast(HitResult, Direction);
		}
	}
}
