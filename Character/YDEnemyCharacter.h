// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/YDCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Interfaces/YDTargeting.h"
#include "Components/WidgetComponent.h"
#include "Components/YDRotationComponent.h"
#include "Engine/TargetPoint.h"
#include "Equipments/YDWeapon.h"
#include "Interfaces/YDCombatInterface.h"
#include "YDEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class YD_API AYDEnemyCharacter : public AYDCharacterBase, public IYDTargeting, public IYDCombatInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> TargetSphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UYDRotationComponent> RotationComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> LockOnWidgetComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;

protected:
	UPROPERTY(EditAnywhere, Category = "AI | Patrol")
	TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "AI | Patrol")
	int32 PatrolIndex = 0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AYDWeapon>> DefaultWeaponClasses;

protected:
	FTimerHandle ParriedDelayTimerHandle;
	FTimerHandle StunnedDelayTimerHandle;

	UPROPERTY(EditAnywhere)
	int StunnedRate = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Phase")
	TMap<float, EYDCombatType> PhaseRule;

public:
	AYDEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public :
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void OnDeath() override;
	virtual void HitReaction(const AActor* Attacker) override;
	void SetDeathState();
	void OnAttributeChanged(EYDAttributeType AttributeType, float InValue);
	void SetupHealthBar();

public:
	virtual void OnTargeted(bool bTargeted) override;
	virtual bool CanBeTargeted() override;

	virtual void ActivateWeaponCollision(EWeaponCollisionType InWeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType InWeaponCollisionType) override;
	virtual void PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) override;
	virtual void Parried() override;
	void ToggleHealthBarVisibility(bool bVisibility);

	virtual void SeesTarget(AActor* InTargetActor);

protected:
	virtual void ApplyStat() override;

public:
	FORCEINLINE ATargetPoint* GetPatrolPoint()
	{
		return PatrolPoints.Num() >= (PatrolIndex + 1) ? PatrolPoints[PatrolIndex] : nullptr;
	}

	FORCEINLINE void IncrementPatrolIndex()
	{
		PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
	}

private :
	void EquipWeapon();
};
