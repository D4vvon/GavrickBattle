// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_PlayerController.h"
#include "../GB_BaseCharacter.h"
#include "Blueprint/UserWidget.h"
#include "../../UI/Widgets/ReticleWidget.h"
#include "../../UI/Widgets/PlayerHUDWidget.h"
#include "../../UI/Widgets/AmmoWidget.h"
#include "../CharacterComponents/CharacterEquipmentComponent.h"

void AGB_PlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedBaseCharacter = Cast<AGB_BaseCharacter>(InPawn);

	CreateAndInitializeWidgets();
}

void AGB_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AGB_PlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGB_PlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AGB_PlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AGB_PlayerController::LookUp);

	InputComponent->BindAction("InteractWithLadder", EInputEvent::IE_Pressed, this, &AGB_PlayerController::InteractWithLadder);
	InputComponent->BindAction("Mantle", EInputEvent::IE_Pressed, this, &AGB_PlayerController::Mantle);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AGB_PlayerController::Jump);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AGB_PlayerController::ChangeCrouchState);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AGB_PlayerController::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AGB_PlayerController::StopSprint);

	InputComponent->BindAxis("SwimForward", this, &AGB_PlayerController::SwimForward);
	InputComponent->BindAxis("SwimRight", this, &AGB_PlayerController::SwimRight);
	InputComponent->BindAxis("SwimUp", this, &AGB_PlayerController::SwimUp);

	InputComponent->BindAxis("ClimbLadderUp", this, &AGB_PlayerController::ClimbLadderUp);

	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AGB_PlayerController::StartingFire);
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AGB_PlayerController::StopingFire);

	InputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &AGB_PlayerController::StartAiming);
	InputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &AGB_PlayerController::StopAiming);

	InputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &AGB_PlayerController::Reload);

	InputComponent->BindAction("NextItem", EInputEvent::IE_Pressed, this, &AGB_PlayerController::NextItem);
	InputComponent->BindAction("PreviousItem", EInputEvent::IE_Pressed, this, &AGB_PlayerController::PreviousItem);
	InputComponent->BindAction("ThirdItem", EInputEvent::IE_Pressed, this, &AGB_PlayerController::MeleeItem);

	InputComponent->BindAction("EquipPrimaryItem", EInputEvent::IE_Pressed, this, &AGB_PlayerController::EquipPrimaryItem);

	InputComponent->BindAction("PrimaryMeleeAttack", EInputEvent::IE_Pressed, this, &AGB_PlayerController::PrimaryMeleeAttack);
	InputComponent->BindAction("SecondaryMeleeAttack", EInputEvent::IE_Pressed, this, &AGB_PlayerController::SecondaryMeleeAttack);
}

void AGB_PlayerController::MoveForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(Value);
	}
}

void AGB_PlayerController::MoveRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(Value);
	}
}

void AGB_PlayerController::Turn(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(Value * AimTurnRate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AGB_PlayerController::LookUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(Value * AimLookUpRate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AGB_PlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

void AGB_PlayerController::StartSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartSprint();
	}
}

void AGB_PlayerController::StopSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopSprint();
	}
}

void AGB_PlayerController::SwimForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimForward(Value);
	}
}

void AGB_PlayerController::SwimRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimRight(Value);
	}
}

void AGB_PlayerController::SwimUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimUp(Value);
	}
}

void AGB_PlayerController::Mantle()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Mantle(true);
	}
}

void AGB_PlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}

void AGB_PlayerController::ClimbLadderUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ClimbLadderUp(Value);
	}
}

void AGB_PlayerController::InteractWithLadder()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->InteractWithLadder();
	}
}

void AGB_PlayerController::StartingFire()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartFire();
	}
}

void AGB_PlayerController::StopingFire()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopFire();
	}
}

void AGB_PlayerController::StartAiming()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartAiming();
		AimTurnRate = 0.5f;
	}
}

void AGB_PlayerController::StopAiming()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopAiming();
		AimTurnRate = 1.0f;
	}
}

void AGB_PlayerController::Reload()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ReloadCurrebtRangeWeapon();
	}
}

void AGB_PlayerController::NextItem()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->NextItem();
	}
}

void AGB_PlayerController::PreviousItem()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->PreviousItem();
	}
}

void AGB_PlayerController::MeleeItem()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MeleeItem();
	}
}

void AGB_PlayerController::EquipPrimaryItem()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->EquipPrimaryItem();
	}
}

void AGB_PlayerController::PrimaryMeleeAttack()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->PrimaryMeleeAttack();
	}
}

void AGB_PlayerController::SecondaryMeleeAttack()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SecondaryMeleeAttack();
	}
}

void AGB_PlayerController::CreateAndInitializeWidgets()
{
	if (!IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	if (IsValid(PlayerHUDWidget) && CachedBaseCharacter.IsValid())
	{
		PlayerHUDWidget->AddToViewport();
		UReticleWidget* ReticleWidget = PlayerHUDWidget->GetReticleWidget();
		if (IsValid(ReticleWidget))
		{
			CachedBaseCharacter->OnAimingStateChanged.AddUFunction(ReticleWidget, FName("OnAimingStateChange"));
			UCharacterEquipmentComponent* CharacterEquipment = CachedBaseCharacter->GetCharacterEquipmentComponent_Mutable();
			CharacterEquipment->OnEquippedItemChanged.AddUFunction(ReticleWidget, FName("OnEquippedItemChanged"));
		}

		UAmmoWidget* AmmoWidget = PlayerHUDWidget->GetAmmoWidget();
		if (IsValid(AmmoWidget))
		{
			UCharacterEquipmentComponent* CharacterEquipment = CachedBaseCharacter->GetCharacterEquipmentComponent_Mutable();
			CharacterEquipment->OnCurrentWeaponAmmoChangedEvent.AddUFunction(AmmoWidget, FName("UpdateAmmoCount"));
		}
	}
}
