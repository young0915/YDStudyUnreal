// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_YDComboWindow.h"

#include "Character/YDPlayerCharacter.h"

UAnimNotifyState_YDComboWindow::UAnimNotifyState_YDComboWindow(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotifyState_YDComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AYDPlayerCharacter* Character = Cast<AYDPlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->EnableCombWindow();
	}

}

void UAnimNotifyState_YDComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AYDPlayerCharacter* Character = Cast<AYDPlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->DisableCombWindow();
	}
}
