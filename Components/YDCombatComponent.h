// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Cores/YDDefine.h"
#include "Equipments/YDEquipment.h"
#include "Equipments/YDShield.h"
#include "YDCombatComponent.generated.h"

class AYDWeapon;

DECLARE_MULTICAST_DELEGATE(FDelegateOnPlayEquipMontage)
DECLARE_MULTICAST_DELEGATE(FDelegateOnChangedWeapon)


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YD_API UYDCombatComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	FDelegateOnPlayEquipMontage OnPlayEquipMontage;
	FDelegateOnChangedWeapon OnChangedWeapon;
	
protected:
	UPROPERTY()
	AYDWeapon* MainWeapon;

	UPROPERTY()
	AYDShield* Shield;

	UPROPERTY(EditAnywhere)
	TMap<EYDCombatType, TObjectPtr<AYDWeapon>> EquippedWeapons;

	UPROPERTY(VisibleAnywhere)
	EYDCombatType CurrentCombatType = EYDCombatType::Unarmed;

	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;

	UPROPERTY()
	bool bBlockingEnabled = false;

	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;

public:
	UYDCombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetWeapon(AYDWeapon* NewWeapon);
	void SetShield(AYDShield* NewShield);
	void SwapWeapon(EYDCombatType InCombatType= EYDCombatType::Unarmed);
	void AddWeapon(EYDCombatType InCombatTYpe, AYDWeapon* InWeapon);
	void EquipWeapon(int32 InItemCode, bool InBEquip);

public:
	FORCEINLINE AYDWeapon* GetMainWeapon() const { return MainWeapon; }
	FORCEINLINE AYDShield* GetShield() const { return Shield; }

	FORCEINLINE bool IsBlockingEnabled() const { return bBlockingEnabled; }
	FORCEINLINE void SetBlockingEnabled(bool bBlocking) { bBlockingEnabled = bBlocking; }

	FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; }
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackTypeTag) { LastAttackType = NewAttackTypeTag; }
	FORCEINLINE TMap<EYDCombatType, TObjectPtr<AYDWeapon>> GetEquippedWeapons() const { return EquippedWeapons; }

	bool HasMultipleWeapons();
};
