// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "../../Components/Weapons/WeaponBarrel.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/AIModule/Classes/AIController.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/AIModule/Classes/GenericTeamAgentInterface.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* TurretRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Turret root"));
	SetRootComponent(TurretRootComponent);

	TurretBaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Turret base"));
	TurretBaseComponent->SetupAttachment(TurretRootComponent);

	TurretBarellComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Turret barell"));
	TurretBarellComponent->SetupAttachment(TurretBaseComponent);

	WeaponBarell = CreateDefaultSubobject<UWeaponBarrel>(TEXT("Weapon barell"));
	WeaponBarell->SetupAttachment(TurretBarellComponent);

}

void ATurret::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AAIController* AIController = Cast<AAIController>(NewController);
	if (IsValid(AIController))
	{
		FGenericTeamId TeamId((uint8)Team);
		AIController->SetGenericTeamId(TeamId);
	}
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentTurretState)
	{
		case ETurretState::Searching:
		{
			//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, FString::Printf(TEXT("Search")), true);
			SearchingMovement(DeltaTime);
			break;
		}
		case ETurretState::Firing:
		{
			FiringMovement(DeltaTime);
			break;
		}
		default:
			break;
	}

}

void ATurret::SetCurrentTarget(AActor* NewTarget)
{
	CurrentTarget = NewTarget;
	ETurretState NewState = IsValid(CurrentTarget) ? ETurretState::Firing : ETurretState::Searching;
	SetCurrentTurretState(NewState);
}

FVector ATurret::GetPawnViewLocation() const
{
	return WeaponBarell->GetComponentLocation();
}

FRotator ATurret::GetViewRotation() const
{
	return WeaponBarell->GetComponentRotation();
}

void ATurret::SearchingMovement(float DeltaTime)
{
	FRotator TurretBaseRotation = TurretBaseComponent->GetRelativeRotation();
	TurretBaseRotation.Yaw += DeltaTime * BaseSearchingRotationRate;
	TurretBaseComponent->SetRelativeRotation(TurretBaseRotation);

	FRotator TurretBarellRotation = TurretBarellComponent->GetRelativeRotation();
	TurretBarellRotation.Pitch += FMath::FInterpTo(TurretBarellRotation.Pitch, 0.0f, DeltaTime, BarellPitchRotationRate);
	TurretBarellComponent->SetRelativeRotation(TurretBarellRotation);
}

void ATurret::FiringMovement(float DeltaTime)
{
	FVector BaseLookAtDirection = (CurrentTarget->GetActorLocation() - TurretBaseComponent->GetComponentLocation()).GetSafeNormal2D();
	FQuat LookAtQuat = BaseLookAtDirection.ToOrientationQuat();
	FQuat TargetQuat = FMath::QInterpTo(TurretBaseComponent->GetComponentQuat(), LookAtQuat, DeltaTime, BaseFiringInterpSpeed);
	TurretBaseComponent->SetWorldRotation(TargetQuat);

	FVector BarellLookAtDirection = (CurrentTarget->GetActorLocation() - TurretBarellComponent->GetComponentLocation()).GetSafeNormal();
	float BarellLookPitchAngle = BaseLookAtDirection.ToOrientationRotator().Pitch;

	FRotator BarellLocalRotation = TurretBarellComponent->GetRelativeRotation();
	BarellLocalRotation.Pitch = FMath::FInterpTo(BarellLocalRotation.Pitch, BarellLookPitchAngle, DeltaTime, BarellPitchRotationRate);
	TurretBarellComponent->SetRelativeRotation(BarellLocalRotation);
}

void ATurret::SetCurrentTurretState(ETurretState NewState)
{
	bool bIsStateChanged = NewState != CurrentTurretState;
	CurrentTurretState = NewState;
	if (!bIsStateChanged)
	{
		return;
	}

	switch (CurrentTurretState)
	{
		case ETurretState::Searching:
		{
			GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
			break;
		}
		case ETurretState::Firing:
		{
			GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ATurret::MakeShot, GetFireInterval(), true);
			break;
		}
	}
}

float ATurret::GetFireInterval()
{
	return 60.0f / RateOfFire;
}

void ATurret::MakeShot()
{
	FVector ShotLocation = WeaponBarell->GetComponentLocation();
	FVector ShotDirection = WeaponBarell->GetComponentRotation().RotateVector(FVector::ForwardVector);
	float SpreadAngle = FMath::DegreesToRadians(BulletSpreadAngle);
	WeaponBarell->Shot(ShotLocation, ShotDirection, SpreadAngle);
}
