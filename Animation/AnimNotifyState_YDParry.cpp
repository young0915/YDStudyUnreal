// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_YDParry.h"
#include "Components/YDStateComponent.h"

void UAnimNotifyState_YDParry::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UYDStateComponent* StateComponent = OwnerActor->GetComponentByClass<UYDStateComponent>())
		{
			StateComponent->SetState(YDGameplayTags::Character_State_Parrying);
		}
	}
}

void UAnimNotifyState_YDParry::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UYDStateComponent* StateComponent = OwnerActor->GetComponentByClass<UYDStateComponent>())
		{
			StateComponent->ClearState();
		}
	}

}
