// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/YDEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/YDPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

AYDEnemyAIController::AYDEnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
}

void AYDEnemyAIController::StopUpdateTarget()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	SetTarget(nullptr);
}

void AYDEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ControlledEnemy = Cast<AYDEnemyCharacter>(InPawn);

	RunBehaviorTree(BehaviorTreeArsset);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateTarget, 0.1f, true);
}

void AYDEnemyAIController::OnUnPossess()
{
	StopUpdateTarget();
	ControlledEnemy = nullptr;
	Super::OnUnPossess();
}

void AYDEnemyAIController::UpdateTarget() const
{
	TArray<AActor*> OutActors;
	AIPerceptionComponent->GetKnownPerceivedActors(nullptr, OutActors);
	
	AYDPlayerCharacter* PlayerCharacter = Cast<AYDPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (OutActors.Contains(PlayerCharacter))
	{
		if (!PlayerCharacter->IsDeath())
		{
			SetTarget(PlayerCharacter);
			ControlledEnemy->ToggleHealthBarVisibility(true);
			ControlledEnemy->SeesTarget(PlayerCharacter);
		}
		else
		{
			SetTarget(nullptr);
			ControlledEnemy->ToggleHealthBarVisibility(false);
		}
	}
	else
	{
		SetTarget(nullptr);
		ControlledEnemy->ToggleHealthBarVisibility(false);
	}
}

void AYDEnemyAIController::SetTarget(AActor* NewActor) const
{
	if (IsValid(Blackboard))
	{
		Blackboard->SetValueAsObject(FName("Target"), NewActor);
	}

	if (IsValid((ControlledEnemy)))
	{
		if (UYDRotationComponent* RotationComponent= ControlledEnemy->GetComponentByClass<UYDRotationComponent>())
		{
			RotationComponent->SetTargetActor(NewActor);
		}	
	}
	
}
