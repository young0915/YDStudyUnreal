// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/YDShield.h"

#include "YDWeapon.h"
#include "Components/YDCombatComponent.h"

void AYDShield::EquipItem()
{
	Super::EquipItem();

	if (const AActor* OwnerActor = Cast<AActor>(GetOwner()))
	{
		if (UYDCombatComponent* CombatComp = OwnerActor->GetComponentByClass<UYDCombatComponent>())
		{
			CombatComp->SetShield(this);

			if ( AYDWeapon* MainWeapon = CombatComp->GetMainWeapon())
			{
				if (MainWeapon->GetCombatType() == EYDCombatType::SwordShield)
				{
					if (!EquipSocketName.IsNone())
					{
						AttachToOwner(EquipSocketName);
					}
				}
			}
		}
	}
}
