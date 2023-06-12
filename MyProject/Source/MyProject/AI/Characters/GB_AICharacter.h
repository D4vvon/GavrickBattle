// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Characters/GB_BaseCharacter.h"
#include "GB_AICharacter.generated.h"

class UAIPatrollingComponent;
UCLASS(Blueprintable)
class MYPROJECT_API AGB_AICharacter : public AGB_BaseCharacter
{
	GENERATED_BODY()
	
public:
	AGB_AICharacter(const FObjectInitializer& ObjectInitializer);

	UAIPatrollingComponent* GetPatrollingComponent() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPatrollingComponent* AIPatrollingComponent;
};
