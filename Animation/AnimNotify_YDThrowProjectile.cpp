// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_YDThrowProjectile.h"
#include "Components/YDInventoryComponent.h"


void UAnimNotify_YDThrowProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UYDInventoryComponent* InventoryComp = OwnerActor->GetComponentByClass<UYDInventoryComponent>())
		{
			InventoryComp->ThrowProjectile();
		}
	}
}
