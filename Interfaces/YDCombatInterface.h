// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cores/YDDefine.h"
#include "UObject/Interface.h"
#include "YDCombatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UYDCombatInterface : public UInterface
{
	GENERATED_BODY()
};

struct FGameplayTag;

class YD_API IYDCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ActivateWeaponCollision(EWeaponCollisionType InWeaponCollisionType) =0;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType InWeaponCollisionType) =0;
	virtual void PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) {}
	virtual void Parried() {}
};
