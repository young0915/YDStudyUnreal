// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cores/YDDefine.h"
#include "Components/ActorComponent.h"
#include "YDAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnAttributeChanged, EYDAttributeType, float);
DECLARE_MULTICAST_DELEGATE(FOnDeath)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YD_API UYDAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FDelegateOnAttributeChanged OnAttributeChanged;
	FOnDeath OnDeath;
	
protected:
	UPROPERTY(EditAnywhere, Category="Stamina")
	float BaseStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category="Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category="Stamina")
	float StaminaRegenRate = 0.2f;

	FTimerHandle StaminaRegenTimerHandle;


	UPROPERTY(EditAnywhere, Category="Health")
	float BaseHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth = 100.0f;

	UPROPERTY()
	float DefenseStat = 0.f;
public:
	UYDAttributeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetBaseStamina() { return BaseStamina; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetBaseHealth() { return BaseHealth; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }

	FORCEINLINE void IncreaseDefense(const float DefenseAmount)
	{
		DefenseStat += DefenseAmount;
	}

	FORCEINLINE void DecraseDefense(const float DefenseAmount)
	{
		DefenseStat -= DefenseAmount;
	}

	FORCEINLINE float GetStatimaRatio() { return BaseStamina / MaxStamina; }
	FORCEINLINE float GetHealthRatio() { return BaseHealth / MaxHealth; }

	bool CheckHasEnoughStamina(float StaminaCost) const;

	void DecreaseStamina(float StaminaCost);

	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 2.0f);

	void BroadcastAttributeChanged(EYDAttributeType InAttributeType);

	void TakeDamageAmount(float DamageAmount);

	void Heal(float HealAmount);

	void ApplyAttribute(float InHealth, float InStamina, float InStaminaRegenRate, float InDefense);
private:
	void RegenerateStaminaHandler();
};
