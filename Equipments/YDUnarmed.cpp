// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/YDUnarmed.h"

#include "Animation/YDAnimInstance.h"
#include "Character/YDPlayerCharacter.h"

AYDUnarmed::AYDUnarmed()
{
}

void AYDUnarmed::EquipItem()
{
	CombatComponent = GetOwner()->GetComponentByClass<UYDCombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);
		
		if (AYDPlayerCharacter* OwnerCharacter = Cast<AYDPlayerCharacter>(GetOwner()))
		{
			WeaponCollision->SetWeaponMesh(OwnerCharacter->GetMesh());
			SecondWeaponCollision->SetWeaponMesh(OwnerCharacter->GetMesh());

			if (UYDAnimInstance* Anim = Cast<UYDAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}

			WeaponCollision->AddIgnoreActor(OwnerCharacter);
			SecondWeaponCollision->AddIgnoreActor(OwnerCharacter);
		}
	}
}
