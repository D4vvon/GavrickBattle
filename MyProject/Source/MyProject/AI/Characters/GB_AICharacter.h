// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Characters/GB_BaseCharacter.h"
#include "GB_AICharacter.generated.h"

class UAIPatrollingComponent;
class UBehaviorTree;
UCLASS(Blueprintable)
class MYPROJECT_API AGB_AICharacter : public AGB_BaseCharacter
{
	GENERATED_BODY()
	
public:
	AGB_AICharacter(const FObjectInitializer& ObjectInitializer);

	UAIPatrollingComponent* GetPatrollingComponent() const;

	UBehaviorTree* GetBehaviorTree() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPatrollingComponent* AIPatrollingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
};
