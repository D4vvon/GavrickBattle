// Fill out your copyright notice in the Description page of Project Settings.


#include "Su33_AnimInstanse.h"
#include "../GB_BasePlanePawn.h"

void USu33_AnimInstanse::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	checkf(TryGetPawnOwner()->IsA<AGB_BasePlanePawn>(), TEXT("Only with BasePlanePawn"));
	CachedSu33_Pawn = StaticCast<AGB_BasePlanePawn*>(TryGetPawnOwner());
}

void USu33_AnimInstanse::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedSu33_Pawn.IsValid())
	{
		return;
	}

	
}
