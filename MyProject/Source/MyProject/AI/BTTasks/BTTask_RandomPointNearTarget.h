// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomPointNearTarget.generated.h"

UCLASS()
class MYPROJECT_API UBTTask_RandomPointNearTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RandomPointNearTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector LocationKey;
};
