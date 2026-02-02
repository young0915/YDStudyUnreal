// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_SelectBehaviorBoss.h"

#include "BehaviorTree/BlackboardComponent.h"

void UBTService_SelectBehaviorBoss::UpdateBehavior(UBlackboardComponent* BlackboardComp,
                                                   AYDEnemyCharacter* ControlledEnemy) const
{
	check(BlackboardComp);
	check(ControlledEnemy);

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	if (IsValid(TargetActor))
	{
		if (UYDAttributeComponent* AttributeComponent = ControlledEnemy->GetComponentByClass<UYDAttributeComponent>())
		{
			if (AttributeComponent->GetBaseStamina() <= StaminaCheckValue)
			{
				SetBehaviorKey(BlackboardComp, EYDAIBehavior::Strafe);
			}
			else
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
		}
	}
	else
	{
		SetBehaviorKey(BlackboardComp, EYDAIBehavior::Idle);
	}
}
