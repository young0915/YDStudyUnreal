// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/YDInventoryComponent.h"
#include "Components/YDWeaponCollisionComponent.h"
#include "Datas/YDMontageActionData.h"
#include "Equipments/YDEquipment.h"
#include "YDWeapon.generated.h"

class UYDItemObject;
class UYDCombatComponent;
class UYDInventoryComponent;

/**
 * 
 */
UCLASS()
class YD_API AYDWeapon : public AYDEquipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName EquipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName UnequipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Animation")
	TObjectPtr<UYDMontageActionData> MontageActionData;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMeshComponent> LeftWeaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Sub Equipment")
	TSubclassOf<AYDEquipment> SubEquipmentClass;
	
protected:
	UPROPERTY(VisibleAnywhere)
	UYDWeaponCollisionComponent* WeaponCollision;

	UPROPERTY(VisibleAnywhere)
	UYDWeaponCollisionComponent* SecondWeaponCollision;
	
protected:
	UPROPERTY()
	UYDCombatComponent* CombatComponent;

	UPROPERTY()
	UYDInventoryComponent* InventoryComponent;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Data")
	TMap<FGameplayTag, float> StaminaCostMap;

	UPROPERTY(EditAnywhere)
	float BaseDamage = 15.0f;

	UPROPERTY(EditAnywhere)
	float Defense =0.0f;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> DamageMultiplierMap;

	UPROPERTY(EditAnywhere, Category= "Equipment | Sub Weapon Mesh")
	TObjectPtr<UStaticMesh> SubMeshAsset;

	UPROPERTY(VisibleAnywhere)
	UYDItemObject* ItemRef = nullptr;
	
public:
	AYDWeapon();

public:
	virtual auto BeginPlay() -> void override;
	virtual void EquipItem() override;
	virtual void CreateItem() override;
	void Drop();
	void EnsureOffHandEquipped();
	void ApplyStat();

	UAnimMontage* GetMontageForTag(const FGameplayTag& Tag, const int32 Index = 0) const;
	UAnimMontage* GetRandomMontageForTag(const FGameplayTag& Tag, const int32 Index = 0) const;
	UAnimMontage* GetHitReactMontage(const AActor* Attacker) const;

	float GetStaminaCost(const FGameplayTag& InTag) const;
	float GetAttackDamage() const;
	bool GetIsEquip() const;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocket() const { return UnequipSocketName; }
	FORCEINLINE void SetEquipScokectName(FName InSocketName) {EquipSocketName = InSocketName;}
	FORCEINLINE UYDWeaponCollisionComponent* GetCollision() const { return WeaponCollision; }
	FORCEINLINE void SetItemInfo(UYDItemObject* InItem) {ItemRef = InItem;}
	FORCEINLINE UYDItemObject* GetItemRef() const { return ItemRef; }
	FORCEINLINE float GetDefense() const { return Defense; }
	
public:
	virtual void ActivateCollision(EWeaponCollisionType  InCollisionType);
	virtual void DeactivateCollision(EWeaponCollisionType  InCollisionType);

public:
	void OnHitActor(const FHitResult& Hit);
};
