// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_BaseCharacterAnimInstance.h"
#include "../GB_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGB_BaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AGB_BaseCharacter>(), TEXT("UGB_BaseCharacterAnimInstance::NativeBeginPlay() can used only with AGB_BaseCharacter"))
	CachedBaseCharacter = StaticCast<AGB_BaseCharacter*>(TryGetPawnOwner());
}

void UGB_BaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}

	UCharacterMovementComponent* CharacterMovementComponent = CachedBaseCharacter->GetCharacterMovement();
	Speed = CharacterMovementComponent->Velocity.Size();
	bIsFalling = CharacterMovementComponent->IsFalling();
	bIsCrouching = CharacterMovementComponent->IsCrouching();
}
