// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Cores/YDDefine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "YDAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YD_API UYDAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TObjectPtr<ACharacter> Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
	bool bCombatEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
	EYDCombatType CombatType = EYDCombatType::Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
	bool bShouldBlocking = false;

public:
	UYDAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public:
	UFUNCTION()
	void AnimNotify_ResetMovementInput();

	UFUNCTION()
	void AnimNotify_ResetState();
	
public:
	void UpdateCombatMode(const EYDCombatType InCombatType);
	void UpdateBlocking(bool InShouldBlocking);

protected:
	void OnChangedCombat(const bool bInCombatEnabled);
};

