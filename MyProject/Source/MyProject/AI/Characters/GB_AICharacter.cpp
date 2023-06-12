// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_AICharacter.h"
#include "../../Characters/CharacterComponents/AIPatrollingComponent.h"

AGB_AICharacter::AGB_AICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIPatrollingComponent = CreateDefaultSubobject<UAIPatrollingComponent>(TEXT("AIPatrolling"));
}

UAIPatrollingComponent* AGB_AICharacter::GetPatrollingComponent() const
{
	return AIPatrollingComponent;
}
