// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_YDEquipWeapon.h"

#include "Components/YDCombatComponent.h"
#include "Cores/YDGameplayTags.h"
#include "Equipments/YDWeapon.h"

UAnimNotify_YDEquipWeapon::UAnimNotify_YDEquipWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UAnimNotify_YDEquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (UYDCombatComponent* CombatComponent = Owner->GetComponentByClass<UYDCombatComponent>())
		{
			if (AYDWeapon* MainWeapon = CombatComponent->GetMainWeapon())
			{
				MainWeapon->AttachToOwner(MainWeapon->GetEquipSocketName());
			}
		}
	}
}
