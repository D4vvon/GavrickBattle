// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MovementComponents/GB_BaseCharacterMovementComp.h"
#include "MovementComponents/LedgeDetectorComponent.h"
#include "Curves/CurveVector.h"
#include "../Actor/Environment/Ladder.h"
#include "CharacterComponents/CharacterAttributesComponent.h"
#include "../GavrickBattleTypes.h"
#include "Engine/DamageEvents.h"
#include "CharacterComponents/CharacterEquipmentComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "../Actor/Equipment/Weapons/RangeWeaponItem.h"
#include "../Actor/Equipment/Weapons/MeleeWeaponItem.h"
#include "AIController.h"

AGB_BaseCharacter::AGB_BaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGB_BaseCharacterMovementComp>(ACharacter::CharacterMovementComponentName))
{
	GB_BaseCharacterMovementComp = StaticCast<UGB_BaseCharacterMovementComp*>(GetCharacterMovement());
	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));
	CharacterAttributesComponent = CreateDefaultSubobject<UCharacterAttributesComponent>(TEXT("CharacterAttributes"));
	CharacterEquipmentComponent = CreateDefaultSubobject<UCharacterEquipmentComponent>(TEXT("CharacterEquipment"));
}

void AGB_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterAttributesComponent->OnDeathEvent.AddUObject(this, &AGB_BaseCharacter::OnDeath);

	//CharacterEquipmentComponent->EquipSidearmWeaponItem();
}

void AGB_BaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AAIController* AIController = Cast<AAIController>(NewController);
	if (IsValid(AIController))
	{
		FGenericTeamId TeamId((uint8)Team);
		AIController->SetGenericTeamId(TeamId);
	}
}

void AGB_BaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AGB_BaseCharacter::StartSprint()
{
	if (!IsAiming())
	{
		bIsSprintRequested = true;
	}
	else
	{
		bIsSprintRequested = false;
	}
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void AGB_BaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

void AGB_BaseCharacter::Mantle(bool bForce /*= false*/)
{

	if (!(CanMantling() || bForce ))
	{
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, FString::Printf(TEXT("CantMantle: %s; bForce: %s"),CanMantling(), bForce), true);
		return;
	}

	FLedgeDescription LedgeDescription;
	if (LedgeDetectorComponent->DetectLedge(LedgeDescription))
	{

		FMantlingMovementParameters MantlingParameters;
		MantlingParameters.InitialLocation = GetActorLocation();
		MantlingParameters.InitialRotator = GetActorRotation();
		MantlingParameters.TargetLocation = LedgeDescription.Location;
		MantlingParameters.TartgetRotator = LedgeDescription.Rotation;

		float MantlingHeight = (MantlingParameters.TargetLocation - MantlingParameters.InitialLocation).Z;
		const FMantlingSettings& MantleSettings = GetMantleSettings(MantlingHeight);

		float MinRange;
		float MaxRange;
		MantleSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange);

		MantlingParameters.Duration = MaxRange - MinRange;

		MantlingParameters.MantlingCurve = MantleSettings.MantlingCurve;

		//float StartTime = MantleSettings.MaxHeightStartTime + (MantlingHeight - MantleSettings.MinHeight) / (MantleSettings.MaxHeight - MantleSettings.MinHeight) * (MantleSettings.MaxHeightStartTime - MantleSettings.MinHeightStartTime);
		
		FVector2D SourceRange(MantleSettings.MinHeight, MantleSettings.MaxHeight);
		FVector2D TargetRange(MantleSettings.MinHeightStartTime, MantleSettings.MaxHeightStartTime);
		MantlingParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, MantlingHeight);

		MantlingParameters.InitialAnimationLocation = MantlingParameters.TargetLocation - MantleSettings.AnimationCorrectionZ * FVector::UpVector + MantleSettings.AnimationCorrectionXY * LedgeDescription.LedgeNormal;

		GetBaseCharacterMovementComp()->StartMantle(MantlingParameters);

		UAnimInstance* AnimInstanse = GetMesh()->GetAnimInstance();
		AnimInstanse->Montage_Play(MantleSettings.MantlingMontage, 1.0f, EMontagePlayReturnType::Duration, MantlingParameters.StartTime);
		//PlayAnimMontage(HighMantleMontage);
	}
}

void AGB_BaseCharacter::ClimbLadderUp(float Value)
{
	if (!FMath::IsNearlyZero(Value) && GetBaseCharacterMovementComp()->IsOnLadder())
	{
		FVector LadderUpVector = GetBaseCharacterMovementComp()->GetCurrentLadder()->GetActorUpVector();
		AddMovementInput(LadderUpVector, Value);
	}
}

void AGB_BaseCharacter::InteractWithLadder()
{
	if (GetBaseCharacterMovementComp()->IsOnLadder())
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, TEXT("Detach"), true);
		GetBaseCharacterMovementComp()->DetachFromLadder();
	}
	else
	{
		const ALadder* AvailableLadder = GetAvailableLadder();
		if (IsValid(AvailableLadder))
		{
			if (AvailableLadder->GetIsOnTop())
			{
				PlayAnimMontage(AvailableLadder->GetAttachFromTopAnimMontage());	
			}
			GetBaseCharacterMovementComp()->AttachToLadder(AvailableLadder);
		}
	}
}

const class ALadder* AGB_BaseCharacter::GetAvailableLadder() const
{
	const ALadder* Result = nullptr;

	for (const AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (InteractiveActor->IsA<ALadder>())
		{
			Result = StaticCast<const ALadder*>(InteractiveActor);
			break;
		}
	}
	return Result;
}

bool AGB_BaseCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() && !GB_BaseCharacterMovementComp->IsMantling();
}

void AGB_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryChangeSprintState();
	if (bIsSprintRequested)
	{
		CharacterAttributesComponent->StaminaConsumption();
	}
	else
	{
		CharacterAttributesComponent->SetCanRestoreStamina(true);
	}
	if (CharacterAttributesComponent->GetCurrentStamina() < 5)
	{
		bIsSprintRequested = false;
	}
}

const UCharacterEquipmentComponent* AGB_BaseCharacter::GetCharacterEquipmentComponent() const
{
	return CharacterEquipmentComponent;
}

UCharacterEquipmentComponent* AGB_BaseCharacter::GetCharacterEquipmentComponent_Mutable() const
{
	return CharacterEquipmentComponent;
}

void AGB_BaseCharacter::RegisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.AddUnique(InteractiveActor);
}

void AGB_BaseCharacter::UnregisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.RemoveSingleSwap(InteractiveActor);
}

void AGB_BaseCharacter::Falling()
{
	GetCharacterMovement()->bNotifyApex = true;
}

void AGB_BaseCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	CurrentFallApex = GetActorLocation();
}

void AGB_BaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	float FallHeight = (CurrentFallApex - GetActorLocation()).Z * 0.01f;
	if (IsValid(FallDamageCurve))
	{
		float DamageAmount = FallDamageCurve->GetFloatValue(FallHeight);
		TakeDamage(DamageAmount, FDamageEvent(), GetController(), Hit.GetActor());
	}
}

FGenericTeamId AGB_BaseCharacter::GetGenericTeamId() const
{
	return FGenericTeamId((uint8)Team);
}

void AGB_BaseCharacter::OnSprintStart_Implementation()
{

}

void AGB_BaseCharacter::OnSprintEnd_Implementation()
{

}

bool AGB_BaseCharacter::CanSprint()
{
	float CurrentStamina = CharacterAttributesComponent->GetCurrentStamina();
	if (CurrentStamina > 10.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float AGB_BaseCharacter::GetCurrentStamina()
{
	return CharacterAttributesComponent->GetCurrentStamina();
}

bool AGB_BaseCharacter::CanMantling() const
{
	return GetBaseCharacterMovementComp()->IsOnLadder();
}

void AGB_BaseCharacter::OnDeath()
{
	float Duration = PlayAnimMontage(OnDeathAnimMontage);
	//GetWorld()->GetTimerManager().SetTimer(DeathMontageTimer, this, &AGB_BaseCharacter::EnableRagdoll, Duration, false);
	if (Duration == 0.0f)
	{
		EnableRagdoll();
	}
	GetCharacterMovement()->DisableMovement();
}

void AGB_BaseCharacter::OnStartAimingInternal()
{
	if (OnAimingStateChanged.IsBound())
	{
		OnAimingStateChanged.Broadcast(true);
	}
}

void AGB_BaseCharacter::OnStopAimingInternal()
{
	if (OnAimingStateChanged.IsBound())
	{
		OnAimingStateChanged.Broadcast(false);
	}
}

const FMantlingSettings& AGB_BaseCharacter::GetMantleSettings(float LedgeHeight) const
{
	return LedgeHeight > LowMantleMaxHeight ? HighMantlingSettings : LowMantlingSettings;
}

void AGB_BaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && !GetBaseCharacterMovementComp()->IsOnLadder() && !GB_BaseCharacterMovementComp->IsSprinting() && CanSprint())
	{
		GB_BaseCharacterMovementComp->StartSprint();
		OnSprintStart();
	}

	if (!bIsSprintRequested && GB_BaseCharacterMovementComp->IsSprinting())
	{
		GB_BaseCharacterMovementComp->StopSprint();
		OnSprintEnd();
	}
}

bool AGB_BaseCharacter::GetIsSprintRequest()
{
	return bIsSprintRequested;
}

bool AGB_BaseCharacter::CanFire()
{
	if (CharacterEquipmentComponent->IsEquipping())
	{
		return false;
	}
	else if (GetBaseCharacterMovementComp()->IsSprinting())
	{
		return false;
	}
	else if (GetBaseCharacterMovementComp()->IsMantling())
	{
		return false;
	}
	else
	{
		 return true;
	}

}

void AGB_BaseCharacter::StartFire()
{
	if (!CanFire())
	{
		return;
	}
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();

	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StartFire();
	}
}

void AGB_BaseCharacter::StopFire()
{
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();

	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StopFire();
	}
}

void AGB_BaseCharacter::StartAiming()
{
	ARangeWeaponItem* CurrentRangeWeapon = GetCharacterEquipmentComponent()->GetCurrentRangeWeapon();
	if (!IsValid(CurrentRangeWeapon))
	{
		return;
	}
	bIsAiming = true;
	CurrentAimingMovementSpeed = CurrentRangeWeapon->GetAimMovementMaxSpeed();
	CurrentRangeWeapon->StartAim();
	OnStartAiming();
}

void AGB_BaseCharacter::StopAiming()
{
	if (!bIsAiming)
	{
		return;
	}

	ARangeWeaponItem* CurrentRangeWeapon = GetCharacterEquipmentComponent()->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StopAim();
	}
	
	bIsAiming = false;
	CurrentAimingMovementSpeed = 0;
	OnStopAiming();
}

bool AGB_BaseCharacter::IsAiming() const
{
	return bIsAiming;
}

float AGB_BaseCharacter::GetAimingMovementSpeed() const
{
	return CurrentAimingMovementSpeed;
}

void AGB_BaseCharacter::OnStartAiming_Implementation()
{
	OnStartAimingInternal();
}

void AGB_BaseCharacter::OnStopAiming_Implementation()
{
	OnStopAimingInternal();
}

void AGB_BaseCharacter::ReloadCurrebtRangeWeapon()
{
	if (IsValid(CharacterEquipmentComponent->GetCurrentRangeWeapon()))
	{
		CharacterEquipmentComponent->ReloadCurrentWeapon();
	}
}

void AGB_BaseCharacter::NextItem()
{
	CharacterEquipmentComponent->EquipPrimaryWeaponItem();
}

void AGB_BaseCharacter::PreviousItem()
{
	CharacterEquipmentComponent->EquipSidearmWeaponItem();
}

void AGB_BaseCharacter::MeleeItem()
{
	CharacterEquipmentComponent->EquipMeleeWeapon();
}

void AGB_BaseCharacter::EquipPrimaryItem()
{
	CharacterEquipmentComponent->EquipItemInSlot(EEquipmentSlots::PrimaryItemSlot);
}

void AGB_BaseCharacter::PrimaryMeleeAttack()
{
	AMeleeWeaponItem* CurrentMeleeWeapon = CharacterEquipmentComponent->GetCurrentMeleeWeapon();
	if (IsValid(CurrentMeleeWeapon))
	{
		CurrentMeleeWeapon->StartAttack(EMeleeAttackTypes::PrimaryAttack);
	}
}

void AGB_BaseCharacter::SecondaryMeleeAttack()
{
	AMeleeWeaponItem* CurrentMeleeWeapon = CharacterEquipmentComponent->GetCurrentMeleeWeapon();
	if (IsValid(CurrentMeleeWeapon))
	{
		CurrentMeleeWeapon->StartAttack(EMeleeAttackTypes::SecondaryAttack);
	}
}

void AGB_BaseCharacter::EnableRagdoll()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(CollisionProfileRagdoll);
	GetCapsuleComponent()->SetCollisionProfileName(CollisionProfileRagdoll);
}
