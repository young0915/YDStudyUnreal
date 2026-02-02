// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_YDRotateTo.h"
#include "Components/YDRotationComponent.h"

void UAnimNotifyState_YDRotateTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UYDRotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<UYDRotationComponent>())
	{
		RotationComponent->TogglehouldRotate(true);
	}
}

void UAnimNotifyState_YDRotateTo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UYDRotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<UYDRotationComponent>())
	{
		RotationComponent->TogglehouldRotate(false);
	}
}
