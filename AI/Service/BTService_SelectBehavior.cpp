// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_SelectBehavior.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_SelectBehavior::UBTService_SelectBehavior()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTService_SelectBehavior::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_SelectBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		if (AYDEnemyCharacter* ControlledEnemy = Cast<AYDEnemyCharacter>(ControlledPawn))
		{
			UpdateBehavior(OwnerComp.GetBlackboardComponent(),ControlledEnemy);
		}
	}
}

void UBTService_SelectBehavior::SetBehaviorKey(UBlackboardComponent* BlackboardComp, EYDAIBehavior InAIBehavior) const
{
	BlackboardComp->SetValueAsEnum(BehaviorKey.SelectedKeyName, static_cast<uint8>(InAIBehavior));
}

void UBTService_SelectBehavior::UpdateBehavior(UBlackboardComponent* BlackboardComp, AYDEnemyCharacter* ControlledEnemy) const
{
	check(BlackboardComp);
	check(ControlledEnemy);

	const UYDStateComponent* StateComponent = ControlledEnemy->GetComponentByClass<UYDStateComponent>();
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(YDGameplayTags::Character_State_Parried);
	CheckTags.AddTag(YDGameplayTags::Character_State_Stunned);

	if (StateComponent ->IsCurrentStateEqualsToAny(CheckTags))
	{
		SetBehaviorKey(BlackboardComp, EYDAIBehavior::Stunned);
	}
	else
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

		if (IsValid(TargetActor))
		{
			const float Distance = TargetActor->GetDistanceTo(ControlledEnemy);

			if (Distance <= AttackRangeDistance)
			{
				SetBehaviorKey(BlackboardComp, EYDAIBehavior::MeleeAttack);
			}
			else
			{
				SetBehaviorKey(BlackboardComp, EYDAIBehavior::Approach);
			}
		}
		else
		{
			if (ControlledEnemy->GetPatrolPoint() != nullptr)
			{
				SetBehaviorKey(BlackboardComp, EYDAIBehavior::Patrol);
			}
			else
			{
				SetBehaviorKey(BlackboardComp, EYDAIBehavior::Idle);
			}
		}
	}

}
