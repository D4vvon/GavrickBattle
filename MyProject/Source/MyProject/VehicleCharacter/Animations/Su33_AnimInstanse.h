// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Su33_AnimInstanse.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API USu33_AnimInstanse : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

private:
	TWeakObjectPtr<class AGB_BasePlanePawn> CachedSu33_Pawn;
};
