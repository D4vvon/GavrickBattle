// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributesComponent.h"
#include "../GB_BaseCharacter.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../../SubSystems/DebugSubsystem.h"
#include "../../GavrickBattleTypes.h"
#include "../../../../../../UE_5.1/Engine/Source/Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UCharacterAttributesComponent::UCharacterAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCharacterAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<AGB_BaseCharacter>(), TEXT("UCharacterAttributesComponent::BeginPlay() only with AGB_BaseCharacter"));
	CachedBaseCharacterOwner = StaticCast<AGB_BaseCharacter*>(GetOwner());
	
	Health = MaxHealth;
	Stamina = MaxStamina;

	CachedBaseCharacterOwner->OnTakeAnyDamage.AddDynamic(this, &UCharacterAttributesComponent::OnTakeAnyDamage);
}

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
void UCharacterAttributesComponent::DebugDrawAttributes()
{
	UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	if (!DebugSubsystem->IsCategoryEnabled(DebugCategoryCharacterAttributes))
	{
		return;
	}

	FVector TextLocation = CachedBaseCharacterOwner->GetActorLocation() + (CachedBaseCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 8.0f) * FVector::UpVector;
	DrawDebugString(GetWorld(), TextLocation, FString::Printf(TEXT("Health %.2f"), Health), nullptr, FColor::Green, 0.0f, true);
	
	FVector TextLocationSt = CachedBaseCharacterOwner->GetActorLocation() + (CachedBaseCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 4.0f) * FVector::UpVector;
	DrawDebugString(GetWorld(), TextLocationSt, FString::Printf(TEXT("Stamina %.2f"), Stamina), nullptr, FColor::Blue, 0.0f, true);
}
#endif

void UCharacterAttributesComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	bIsCanRestoreHealth = false;
	bIsCanRestoreStamina = false;

	if (!IsAlive())
	{
		return;
	}


	GetWorld()->GetTimerManager().SetTimer(PostDamangeTimer, this, &UCharacterAttributesComponent::CanRestoreHealth, 7.0f, false);
	//GetWorld()->GetTimerManager().SetTimer(PostDamangeTimer, &UCharacterAttributesComponent::SetCanRestore, 1.0f, false, 7.0f);


	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	if (Health <= 0.0f)
	{
		if (OnDeathEvent.IsBound())
		{
			OnDeathEvent.Broadcast();
		}
	}
}

void UCharacterAttributesComponent::RestoreHealth()
{
	Health += HealthRestoreSpeed * 0.05;
}

void UCharacterAttributesComponent::RestoreStamina()
{
	Stamina += StaminaRestoreSpeed * 0.1;
}

void UCharacterAttributesComponent::CanRestoreHealth()
{
	bIsCanRestoreHealth = true;
	bIsCanRestoreStamina = true;
}


// Called every frame
void UCharacterAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsAlive())
	{
		return;
	}

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	DebugDrawAttributes();
#endif

	if (bIsCanRestoreHealth && Health < MaxHealth)
	{
		RestoreHealth();
	}

	if (bIsCanRestoreStamina && Stamina < MaxStamina)
	{
		RestoreStamina();
	}
}

float UCharacterAttributesComponent::GetCurrentStamina()
{
	return Stamina;
}

void UCharacterAttributesComponent::SetCanRestoreStamina(bool bIsCanRestore)
{
	bIsCanRestoreStamina = bIsCanRestore;
}

void UCharacterAttributesComponent::StaminaConsumption()
{
	Stamina = FMath::Clamp(Stamina - StaminaConsumptionSpeed * 0.2, 0.0f, MaxStamina);
}

bool UCharacterAttributesComponent::IsOutOfStamina()
{
	return Stamina < 20 ? true : false;
}

