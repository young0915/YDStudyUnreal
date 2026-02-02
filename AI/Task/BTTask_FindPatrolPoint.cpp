// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task//BTTask_FindPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/YDEnemyCharacter.h"

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	APawn* ControlledPawn = Comp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}
	if (AYDEnemyCharacter* Character = Cast<AYDEnemyCharacter>(ControlledPawn))
	{
		Comp.GetBlackboardComponent()->SetValueAsVector(BlackboardLocation.SelectedKeyName,
		                                                Character->GetPatrolPoint()->GetActorLocation());
		Character->IncrementPatrolIndex();
		return EBTNodeResult::Succeeded;
	}
	return Super::ExecuteTask(Comp, NodeMemory);
}
