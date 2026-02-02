#pragma once

#define COLLISION_OBJECT_INTERACTION ECC_GameTraceChannel1
#define COLLISION_OBJECT_TARGETING ECC_GameTraceChannel2


UENUM(BlueprintType)
enum class EYDAttributeType : uint8
{
	Stamina,
	Health,
};


UENUM(BlueprintType)
enum class EYDItemType : uint8
{
	None = 0,
	Potion,
	Equipment,
	Projectile
};


UENUM(BlueprintType)
enum class EYDHitDirection : uint8
{
	Front = 0,
	Back,
	Left,
	Right,
	Total
};

UENUM(BlueprintType)
enum class EYDCombatType :uint8
{
	Unarmed = 0,
	SwordShield,
	GreatSword,
	Spear,
	Axe,
};

UENUM(BlueprintType)
enum class EYDMovementMode : uint8
{
	None = 0,
	Walking,
	
};


UENUM(BlueprintType)
enum class ESwitchingDirection :uint8
{
	None = 0,
	Left,
	Right
};

UENUM(BlueprintType)
enum class EWeaponCollisionType : uint8
{
	MainCollision,
	SecondCollision
};

UENUM(BlueprintType)
enum class EYDAIBehavior :uint8
{
	Idle,
	Patrol,
	MeleeAttack,
	Approach,
	Stunned,
	Strafe
};

UENUM(BlueprintType)
enum class EYDDamageType : uint8
{
	HitBack,
	Knockback,
};