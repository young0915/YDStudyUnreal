// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Cores/YDDefine.h"
#include "AnimNotifyState_WeaponCollision.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName ="Weapon Collision"))
class YD_API UAnimNotifyState_WeaponCollision : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponCollisionType CollisionType = EWeaponCollisionType::MainCollision;
public:
	UAnimNotifyState_WeaponCollision(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

