// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_YDKnockBack.generated.h"

/**
 * 
 */
UCLASS()
class YD_API UAnimNotify_YDKnockBack : public UAnimNotify
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	float BaseDamage =10.0f;

	UPROPERTY(EditAnywhere)
	FName LocationSocketName;

	UPROPERTY(EditAnywhere)
	float DamageRadius = 50.0f;

	UPROPERTY(EditAnywhere)
	bool bDrawDebug = false;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;	
};
