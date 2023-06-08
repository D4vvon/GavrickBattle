// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GB_BaseCharacter.generated.h"

USTRUCT(BlueprintType)
struct FMantlingSettings
{

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MantlingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* MantlingCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float MaxHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float MinHeight = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float MaxHeightStartTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float MinHeightStartTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float AnimationCorrectionXY = 65.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float AnimationCorrectionZ = 200.0f;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAimingStateChanged, bool)

class AInteractiveActor;
class UGB_BaseCharacterMovementComp;
class UCharacterAttributesComponent;
class UCharacterEquipmentComponent;

UCLASS(Abstract, NotBlueprintable)
class MYPROJECT_API AGB_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGB_BaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};

	virtual void ChangeCrouchState();
	
	virtual void StartSprint();
	virtual void StopSprint();

	virtual void SwimForward(float Value) {};
	virtual void SwimRight(float Value) {};
	virtual void SwimUp(float Value) {};

	virtual void Mantle(bool bForce = false);
	virtual bool CanJumpInternal_Implementation() const override;

	virtual void ClimbLadderUp(float Value);
	virtual void InteractWithLadder();
	const class ALadder* GetAvailableLadder() const;

	float GetCurrentStamina();
	bool GetIsSprintRequest();

	bool CanFire();
	void StartFire();
	void StopFire();

	void StartAiming();
	void StopAiming();
	bool IsAiming() const;
	float GetAimingMovementSpeed() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OnStartAiming();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OnStopAiming();

	void ReloadCurrebtRangeWeapon();

	void NextItem();

	void PreviousItem();

	void EquipPrimaryItem();

	virtual void Tick(float DeltaTime) override;

	const UCharacterEquipmentComponent* GetCharacterEquipmentComponent() const;
	UCharacterEquipmentComponent* GetCharacterEquipmentComponent_Mutable() const;
	//const UCharacterAttributesComponent* GetCharacterAttributeConponent() const;

	//virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE UGB_BaseCharacterMovementComp* GetBaseCharacterMovementComp() const { return GB_BaseCharacterMovementComp; };
	FORCEINLINE UCharacterAttributesComponent* GetCharacterAttributeConponent() const { return CharacterAttributesComponent; };

	void RegisterInteractiveActor(AInteractiveActor* InteractiveActor);
	void UnregisterInteractiveActor(AInteractiveActor* InteractiveActor);

	virtual void Falling() override;
	virtual void NotifyJumpApex() override;
	virtual void Landed(const FHitResult& Hit) override;

	FOnAimingStateChanged OnAimingStateChanged;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintStart();
	virtual void OnSprintStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintEnd();
	virtual void OnSprintEnd_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	float SprintSpeed = 1200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings HighMantlingSettings;	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings LowMantlingSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling", meta = (ClampMin = "0", UIMin = "0"))
	float LowMantleMaxHeight = 125.0f;

	virtual bool CanSprint();

	bool CanMantling() const;

	UGB_BaseCharacterMovementComp* GB_BaseCharacterMovementComp;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	class ULedgeDetectorComponent* LedgeDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	class UCharacterAttributesComponent* CharacterAttributesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	class UCharacterEquipmentComponent* CharacterEquipmentComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Animations")
	class UAnimMontage* OnDeathAnimMontage;

	// in meters
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Attrubutes")
	class UCurveFloat* FallDamageCurve;

	virtual void OnDeath();

	virtual void OnStartAimingInternal();
	virtual void OnStopAimingInternal();

private:

	const FMantlingSettings& GetMantleSettings(float LedgeHeight) const;

	TArray<AInteractiveActor*> AvailableInteractiveActors;

	void TryChangeSprintState();

	bool bIsSprintRequested = false;

	//FTimerHandle DeathMontageTimer;
	void EnableRagdoll();

	FVector CurrentFallApex;

	bool bIsAiming = false;
	float CurrentAimingMovementSpeed;

};
