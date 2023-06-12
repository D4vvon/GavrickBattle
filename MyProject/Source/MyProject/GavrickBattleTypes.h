#pragma once

#define ECC_Climbing ECC_GameTraceChannel1
#define ECC_InteractionVolume ECC_GameTraceChannel2
#define ECC_Bullet ECC_GameTraceChannel3
#define ECC_Melee ECC_GameTraceChannel4

const FName CollisionProfilePawn = FName("Pawn");
const FName CollisionProfilePawnInteractionVolume = FName("PawnInteractionVolume");
const FName CollisionProfileRagdoll = FName("Ragdoll");
const FName CollisionProfileNoCollision = FName("NoCollision");

const FName DebugCategoryLedgeDetection = FName("LedgeDetection");
const FName DebugCategoryCharacterAttributes = FName("CharacterAttributes");
const FName DebugCategoryRangeWeapon = FName("RangeWeapon");
const FName DebugCategoryMeleeWeapon = FName("MeleeWeapon");

const FName SocketCharacterWeapon = FName("CharacterWeaponSocket");
const FName SocketWeaponMuzzle = FName("MuzzleSocket");
const FName SocketWeaponForegrip = FName("ForegripSocket");
const FName SocketCharacterThrowable = FName("ThrowableSocket");

const FName FXParameterTraceEnd = FName("TraceEnd");

UENUM(BlueprintType)
enum class EEquipableItemType : uint8
{
	None,
	Pistol,
	Rifle,
	Throwable,
	Melee
};

UENUM(BlueprintType)
enum class EAmmunitionType : uint8
{
	None,
	Pistol,
	Rifle,
	Shotgun,
	ExplosiveGrenade,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipmentSlots : uint8
{
	None,
	SideArm,
	PrimaryWeapon,
	SecondaryWeapon,
	PrimaryItemSlot,
	MeleeWeapon,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EReticleType : uint8
{
	None,
	Pistol,
	SniperRifle,
	Rifle,
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMeleeAttackTypes : uint8
{
	None,
	PrimaryAttack,
	SecondaryAttack,
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ETeams : uint8
{
	Player,
	Enemy
};