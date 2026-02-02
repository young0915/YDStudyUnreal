// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/YDAttributeComponent.h"

#include "YDStateComponent.h"
#include "Cores/YDDefine.h"
#include "Cores/YDGameplayTags.h"

UYDAttributeComponent::UYDAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UYDAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UYDAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


bool UYDAttributeComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return BaseStamina >= StaminaCost;
}

void UYDAttributeComponent::DecreaseStamina(float StaminaCost)
{
	BaseStamina = FMath::Clamp(BaseStamina - StaminaCost, 0.0f, MaxStamina);

	BroadcastAttributeChanged(EYDAttributeType::Stamina);
}

void UYDAttributeComponent::ToggleStaminaRegeneration(bool bEnabled, float StartDelay)
{
	if (bEnabled)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimerHandle) == false)
		{ 
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &ThisClass::RegenerateStaminaHandler, 0.1f, true, StartDelay);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
		}
	}
}


void UYDAttributeComponent::BroadcastAttributeChanged(EYDAttributeType InAttributeType)
{
	if (OnAttributeChanged.IsBound())
	{
		float Ratio = 0.f;
		switch (InAttributeType)
		{
		case EYDAttributeType::Stamina:
			Ratio = GetStatimaRatio();
			break;
		case EYDAttributeType::Health:
			Ratio = GetHealthRatio();
			break;
		}
		OnAttributeChanged.Broadcast(InAttributeType, Ratio);
	}
}

void UYDAttributeComponent::TakeDamageAmount(float DamageAmount)
{
	if (DamageAmount <=0 )
	{
		return;
	}
	const float MaxDamage = DamageAmount * (DamageAmount / (DamageAmount + DefenseStat));
	const float TotalDamage = FMath::Clamp(DamageAmount, 0, MaxDamage);

	BaseHealth = FMath::Clamp(BaseHealth - TotalDamage, 0.f, MaxHealth);

	BroadcastAttributeChanged(EYDAttributeType::Health);

	if (BaseHealth <= 0.0f)
	{
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}

		if (UYDStateComponent* StateComp = GetOwner()->FindComponentByClass<UYDStateComponent>())
		{
			StateComp->SetState(YDGameplayTags::Character_State_Death);
		}
	}
}

void UYDAttributeComponent::Heal(float HealAmount)
{
	BaseHealth = FMath::Clamp(BaseHealth+HealAmount, 0.f, MaxHealth);
	BroadcastAttributeChanged(EYDAttributeType::Health);
}

void UYDAttributeComponent::ApplyAttribute(float InHealth, float InStamina, float InStaminaRegenRate, float InDefense)
{
	BaseHealth = InHealth;
	MaxHealth = BaseHealth;
	BaseStamina = InStamina;
	MaxStamina = BaseStamina;
	StaminaRegenRate = InStaminaRegenRate;
	DefenseStat =InDefense;
}

void UYDAttributeComponent::RegenerateStaminaHandler()
{
	BaseStamina =FMath::Clamp(BaseStamina + StaminaRegenRate, 0.0f, MaxStamina);
	BroadcastAttributeChanged(EYDAttributeType::Stamina);

	if (BaseStamina>= MaxStamina)
	{
		ToggleStaminaRegeneration(false);
	}
}
