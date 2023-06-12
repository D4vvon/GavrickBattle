// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GB_AIController.generated.h"

class UAISense;
UCLASS()
class MYPROJECT_API AGB_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AGB_AIController();

protected:
	AActor* GetClosestSensedActor(TSubclassOf<UAISense> SenseClass);
};
