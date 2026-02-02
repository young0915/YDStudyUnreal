// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_YDPerformAttack.h"

#include "AIController.h"
#include "Components/YDStateComponent.h"
#include "Interfaces/YDCombatInterface.h"

EBTNodeResult::Type UBTTask_YDPerformAttack::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	APawn* ControlledPawn = Comp.GetAIOwner()->GetPawn();

	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (IYDCombatInterface* CombatInterface = Cast<IYDCombatInterface>(ControlledPawn))
	{
		FOnMontageEnded MontageEndedDelegate;
		// 델리게이트 바인딩
		MontageEndedDelegate.BindLambda([this, &Comp, ControlledPawn](UAnimMontage* Montage, bool bInterrupted)
			{
				if (::IsValid(&Comp) == false)
				{
					return;
				}

				if (UYDStateComponent* StateComponent = ControlledPawn->GetComponentByClass<UYDStateComponent>())
				{
					FGameplayTagContainer CheckTags;
					CheckTags.AddTag(YDGameplayTags::Character_State_Parried);
					CheckTags.AddTag(YDGameplayTags::Character_State_Stunned);

					if (StateComponent->IsCurrentStateEqualsToAny(CheckTags) == false)
					{
						StateComponent->ClearState();
					}
				}

				FinishLatentTask(Comp, EBTNodeResult::Succeeded);
			});

		CombatInterface->PerformAttack(AttackType, MontageEndedDelegate);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}
