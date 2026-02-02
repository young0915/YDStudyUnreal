// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/YDStateComponent.h"

#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UYDStateComponent::UYDStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UYDStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UYDStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UYDStateComponent::ToggleMovementInput(bool bEnabled, float Duration)
{
	if (bEnabled)
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = "MovementInputEnableAction";
		LatentAction.Linkage = 0;
		LatentAction.UUID = 0;

		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Duration, LatentAction);
	}
	else
	{
		bMovementInputEnabled = false;
	}
}

void UYDStateComponent::MovementInputEnableAction()
{
	bMovementInputEnabled = true;
	ClearState();

}

void UYDStateComponent::ClearState()
{
	if (CurrentState == YDGameplayTags::Character_State_Death)
	{
		return;
	}
	CurrentState = FGameplayTag::EmptyTag;
}

bool UYDStateComponent::IsCurrentStateEqualsToAny(const FGameplayTagContainer& TagsToCheck) const
{
	return TagsToCheck.HasTagExact(CurrentState);
}


