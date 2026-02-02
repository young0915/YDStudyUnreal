// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/YDCombatComponent.h"

#include "Algo/Count.h"
#include "Character/YDBossCharacter.h"
#include "Equipments/YDWeapon.h"
#include "Items/YDPickupItem.h"

UYDCombatComponent::UYDCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UYDCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UYDCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UYDCombatComponent::SetWeapon(AYDWeapon* NewWeapon)
{
	MainWeapon = NewWeapon;
	CurrentCombatType = MainWeapon->GetCombatType();
	AddWeapon(CurrentCombatType, MainWeapon);

	if (OnChangedWeapon.IsBound())
	{
		OnChangedWeapon.Broadcast();
	}
}

void UYDCombatComponent::SetShield(AYDShield* NewShield)
{
	Shield = NewShield;
}

void UYDCombatComponent::SwapWeapon(EYDCombatType InCombatType)
{
	if (::IsValid(MainWeapon))
	{
		MainWeapon->UnEquipItem();
		if (MainWeapon->GetCombatType() == EYDCombatType::SwordShield)
		{
			if (::IsValid(Shield))
			{
				Shield->UnEquipItem();
			}
		}
	}
	
	if (InCombatType != EYDCombatType::Unarmed)
	{
		EquippedWeapons[InCombatType]->EquipItem();
	}
	else
	{
		for (auto weapon : EquippedWeapons)
		{
			if (weapon.Value->GetCombatType() != CurrentCombatType)
			{
				weapon.Value->EquipItem();
				break;
			}
		}
	}
	
	if (OnPlayEquipMontage.IsBound())
	{
		OnPlayEquipMontage.Broadcast();
	}
	if (OnChangedWeapon.IsBound())
	{
		OnChangedWeapon.Broadcast();
	}
}

void UYDCombatComponent::AddWeapon(EYDCombatType InCombatTYpe, AYDWeapon* InWeapon)
{
	if (InCombatTYpe != EYDCombatType::Unarmed && !EquippedWeapons.Contains(InWeapon->GetCombatType()))
	{
		EquippedWeapons.Add(InCombatTYpe, InWeapon);
	}
}

void UYDCombatComponent::EquipWeapon(int32 InItemCode, bool InBEquip)
{
	for (const TPair<EYDCombatType, TObjectPtr<AYDWeapon>>& KV : EquippedWeapons)
	{
		AYDWeapon* Weapon = KV.Value.Get();
		if (Weapon->ItemCode == InItemCode)
		{
			if (UYDItemObject* Item = Weapon->GetItemRef())
			{
				Item->IsEquipped = InBEquip;
				break;
			}
		}
	}

	if (OnChangedWeapon.IsBound())
	{
		OnChangedWeapon.Broadcast();
	}
}

bool UYDCombatComponent::HasMultipleWeapons()
{
	int32 EquippedWeaponsCount = Algo::CountIf(EquippedWeapons,
	                                           [](const TPair<EYDCombatType, TObjectPtr<AYDWeapon>>& Pair)
	                                           {
		                                           const AYDWeapon* Weapon = Pair.Value.Get();
		                                           return IsValid(Weapon) && Weapon->GetIsEquip();
	                                           });

	return (EquippedWeaponsCount >= 2);
}
