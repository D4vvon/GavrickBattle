// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB_AIController.h"
#include "GB_AICharacterController.generated.h"

class AGB_AICharacter;
UCLASS()
class MYPROJECT_API AGB_AICharacterController : public AGB_AIController
{
	GENERATED_BODY()
	
public:
	virtual void SetPawn(APawn* InPawn) override;

	virtual void ActorsPerceptionUpdated(const TArray<AActor *>& UpdatedActors);

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float TargetReachRadius = 100.0f;

	virtual void BeginPlay() override;

private:
	void TryMoveToNextTarget();

	bool IsTargetReached(FVector TargetLocation) const;

	bool bIsPatrolling = false;

	TWeakObjectPtr<AGB_AICharacter> CachedAICharacter;
};
