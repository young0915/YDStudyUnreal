// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_YDAttackFinished.h"

#include "Character/YDPlayerCharacter.h"

UAnimNotify_YDAttackFinished::UAnimNotify_YDAttackFinished(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotify_YDAttackFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AYDPlayerCharacter* Character = Cast<AYDPlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->AttackFinished(ComboResetDelay);
	}
}
