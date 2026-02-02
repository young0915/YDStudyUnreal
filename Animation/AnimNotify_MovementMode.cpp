// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MovementMode.h"

#include "Character/YDPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimNotify_MovementMode::UAnimNotify_MovementMode(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
}

void UAnimNotify_MovementMode::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AYDPlayerCharacter* Character = Cast<AYDPlayerCharacter>(MeshComp->GetOwner()))
	{
		EMovementMode Mode = MOVE_None;
		switch (MovementMode)
		{
		case EYDMovementMode::None:
			break;
		case EYDMovementMode::Walking:
			Mode = MOVE_Walking;
			break;
		}

		Character->SetMovementMode(Mode);
	}
}
