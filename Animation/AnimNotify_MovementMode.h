// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Cores/YDDefine.h"
#include "AnimNotify_MovementMode.generated.h"

/**
 * 
 */
UCLASS(meta= (DisplayName = "YDMovement Mode"))
class YD_API UAnimNotify_MovementMode : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Movement Mode")
	EYDMovementMode MovementMode = EYDMovementMode::None;

public:
	UAnimNotify_MovementMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
