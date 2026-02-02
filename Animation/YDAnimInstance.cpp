// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/YDAnimInstance.h"

#include "YDCharacter.h"
#include "Character/YDPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UYDAnimInstance::UYDAnimInstance()
{
}

void UYDAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());

	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UYDAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Character == nullptr || MovementComponent == nullptr)
	{
		return; 
	}

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();
	bShouldMove = GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;
	bIsFalling = MovementComponent->IsFalling();
}

void UYDAnimInstance::AnimNotify_ResetMovementInput()
{
	if (AYDPlayerCharacter* LocalCharacter = Cast<AYDPlayerCharacter>(GetOwningActor()))
	{
		LocalCharacter->GetStateComponent()->ToggleMovementInput(true);		
	}

}

void UYDAnimInstance::AnimNotify_ResetState()
{
	if (AYDPlayerCharacter* LocalCharacter = Cast<AYDPlayerCharacter>(GetOwningActor()))
	{
		LocalCharacter->GetStateComponent()->ClearState();
	}
}

void UYDAnimInstance::UpdateCombatMode(const EYDCombatType InCombatType)
{
	CombatType = InCombatType;
}

void UYDAnimInstance::UpdateBlocking(bool InShouldBlocking)
{
	bShouldBlocking = InShouldBlocking;
}

void UYDAnimInstance::OnChangedCombat(const bool bInCombatEnabled)
{
	bCombatEnable = bInCombatEnabled;
}
