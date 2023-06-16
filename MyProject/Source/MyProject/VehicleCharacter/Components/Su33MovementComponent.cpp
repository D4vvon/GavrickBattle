// Fill out your copyright notice in the Description page of Project Settings.


#include "Su33MovementComponent.h"
#include "../GB_BasePlanePawn.h"

void USu33MovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateVelocity(DeltaTime);
	PerformMovement(DeltaTime);
	ConsumeInputVector();

	//FHitResult AirHitResult;
	//FVector StartPoint = UpdatedComponent->GetComponentLocation();
	//float TraceLehght = 200.0f;
	//FVector EndPoint = StartPoint - TraceLehght * FVector::UpVector;
	//FCollisionQueryParams IsAirParams;
	//IsAirParams.AddIgnoredActor(GetPlaneOwner());

	//bIsInAir = !GetWorld()->LineTraceSingleByChannel(AirHitResult, StartPoint, EndPoint, ECC_Visibility, IsAirParams);
}

void USu33MovementComponent::SetAcceleration(float Value)
{
	Accelaration = FMath::Clamp(Value, 0.0f, 1.0f);
}

void USu33MovementComponent::SetDeceleration(float Value)
{
	Decelaration = FMath::Clamp(Value, 0.0, 1.0f);
}

void USu33MovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

float USu33MovementComponent::GetMaxSpeed()
{
	return MaxSpeed;
}

bool USu33MovementComponent::MoveUpdatedComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit /* = NULL */, ETeleportType Teleport /* = ETeleportType::None */)
{
	SkelMesh = GetPlaneOwner()->GetSkelMesh();
	if (!bSweep || !IsValid(SkelMesh))
	{
		if (!IsValid(SkelMesh->SkeletalMesh))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("no air")));
			return Super::MoveUpdatedComponentImpl(Delta, NewRotation, bSweep, OutHit, Teleport);
		}
	}

	//FVector SweepStart = SkelMesh->GetComponentLocation() + FVector(0.0f, 0.0, 200.0f);
	FVector SweepStart = SkelMesh->GetComponentLocation();
	FVector SweepEnd = SweepStart + Delta;
	FQuat ComponentRotation = SkelMesh->GetComponentQuat();
	
	TArray<FHitResult> HitResults;
	FComponentQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	FCollisionResponseParams ResponseParams;
	SkelMesh->InitSweepCollisionParams(Params, ResponseParams);

	bool bHasBlockingHit = GetWorld()->ComponentSweepMulti(HitResults, SkelMesh, SweepStart, SweepEnd, NewRotation, Params);
	if (bHasBlockingHit)
	{
		float MinHitTime = 1.0f;
		float Dist = Delta.Size();
		float DesiredTimeBack = FMath::Clamp(0.1f, 0.1f/Dist, 1.0f/Dist) + 0.001f;

		for (FHitResult& Hit : HitResults)
		{
			float HitTime = FMath::Clamp(Hit.Time - DesiredTimeBack, 0.0f, 1.0f);
			if (Hit.bBlockingHit)
			{
				MinHitTime = FMath::Min(HitTime, MinHitTime);
				OutHit = &Hit;
			}
		}
		return UpdatedComponent->MoveComponent(Delta * MinHitTime, NewRotation, false);
	}
	else
	{
		return Super::MoveUpdatedComponentImpl(Delta, NewRotation, false, OutHit, Teleport);
	}
}

FQuat USu33MovementComponent::GetNewRotation(float DeltaTime)
{
	FVector RotationAngles = GetPendingInputVector() * RotationSpeed * DeltaTime;

	FQuat RotationRoll(GetOwner()->GetActorForwardVector(), -FMath::DegreesToRadians(RotationAngles.X));
	FQuat RotationPitch(GetOwner()->GetActorRightVector(), FMath::DegreesToRadians(RotationAngles.Y));
	FQuat RotationYaw(GetOwner()->GetActorUpVector(), FMath::DegreesToRadians(RotationAngles.Z));

	return RotationYaw * RotationRoll * RotationPitch * GetOwner()->GetActorQuat();
}

void USu33MovementComponent::PerformMovement(float DeltaTime)
{
	//FVector LandVelocity = CurrentSpeed * UpdatedComponent->GetForwardVector();
	FVector Delta = Velocity * DeltaTime;
	//FVector Delta = LandVelocity * DeltaTime;
	FHitResult HitResult;
	SafeMoveUpdatedComponent(Delta, GetNewRotation(DeltaTime), true, HitResult);
	//if (!bIsInAir)
	//{
	//}
	//else
	//{
	//	MoveUpdatedComponent(Delta, GetNewRotation(DeltaTime), true);
	//	//FHitResult HitResult;
	//	//SafeMoveUpdatedComponent(Delta, GetNewRotation(DeltaTime), true, HitResult);
	//	//if (!Delta.IsNearlyZero(1e-6f))
	//	//{
	//	//	SafeMoveUpdatedComponent(Delta, GetNewRotation(DeltaTime), true, HitResult);

	//	//	if (HitResult.IsValidBlockingHit())
	//	//	{
	//	//		HandleImpact(HitResult, DeltaTime, Delta);
	//	//		SlideAlongSurface(Delta, 1.0f - HitResult.Time, HitResult.Normal, HitResult, true);
	//	//	}
	//	//}
	//}
}

void USu33MovementComponent::UpdateVelocity(float DeltaTime)
{
	//TargetSpeed = NormalSpeed;
	if (Accelaration > 0.0f)
	{
		//TargetSpeed = FMath::Lerp(NormalSpeed, MaxSpeed, Accelaration);
		if (TargetSpeed < MaxSpeed)
		{
			if (!bIsInAir)
			{
				TargetSpeed = FMath::Lerp(NormalSpeed, NormalSpeed + InterpSpeed, Accelaration);
			}
			else if (bIsInAir)
			{
				TargetSpeed = FMath::Lerp(NormalSpeed, NormalSpeed + AirInterpSpeed, Accelaration);
			}
			NormalSpeed = TargetSpeed;
		}
	}
	else if (Decelaration > 0.0f)
	{
		if (TargetSpeed > 0)
		{
			//TargetSpeed = FMath::Lerp(NormalSpeed, 0.0f, Decelaration);
			if (!bIsInAir)
			{
				TargetSpeed = FMath::Lerp(NormalSpeed, NormalSpeed - InterpSpeed, Decelaration);
			}
			else if (bIsInAir)
			{
				TargetSpeed = FMath::Lerp(NormalSpeed, NormalSpeed - AirInterpSpeed, Decelaration);
			}
			NormalSpeed = TargetSpeed;
		}
	}
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, InterpSpeed);

	if (CurrentSpeed < StallThreshold)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Low speed in air")));
		FallingSpeed += GetWorld()->GetGravityZ() * DeltaTime;
	}
	else
	{
		//FallingSpeed = FMath::FInterpTo(FallingSpeed, 80.0f, DeltaTime, FallingInterpSpeed);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Not air gravity")));
		FallingSpeed = 0.0f;
		
	}
	
	Velocity = GetOwner()->GetActorForwardVector() * CurrentSpeed + FallingSpeed * FVector::UpVector;
	UpdateComponentVelocity();
}

class AGB_BasePlanePawn* USu33MovementComponent::GetPlaneOwner()
{
	return StaticCast<AGB_BasePlanePawn*>(GetOwner());
}
