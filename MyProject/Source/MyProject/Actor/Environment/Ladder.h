// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Interactive/InteractiveActor.h"
#include "Ladder.generated.h"


class UAnimMontage;
class UBoxComponent;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class MYPROJECT_API ALadder : public AInteractiveActor
{
	GENERATED_BODY()
	
public:
	ALadder();
	
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	float GetLadderHeight() const;

	bool GetIsOnTop() const;

	UAnimMontage* GetAttachFromTopAnimMontage() const;
	FVector GetAttachFromTopAnimMontageStartingLocation() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder params", meta = (ClampMin="150", UIMin="150"))
	float LadderHeight = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder params")
	float LadderWidth = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder params")
	float StepsInterval = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder params")
	float BottomStepOffset = 25.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RightRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LeftRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInstancedStaticMeshComponent* StepsMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TopInteractionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder params")
	UAnimMontage* AttachFromTopAnimMontage;

	//Offset from top ladder
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder params")
	FVector AttachFromTopAnimMontageInitialOffset = FVector::ZeroVector;

	UBoxComponent* GetLadderInteractionBox() const;

	virtual void OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	bool bIsOnTop = false;
};
