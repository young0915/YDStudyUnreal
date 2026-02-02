// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Character/YDEnemyCharacter.h"
#include "YDEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class YD_API AYDEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTreeArsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	FTimerHandle TimerHandle;

	UPROPERTY()
	TObjectPtr<AYDEnemyCharacter> ControlledEnemy;

public:
	AYDEnemyAIController();

public:
	void StopUpdateTarget();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	void UpdateTarget() const;
	void SetTarget(AActor* NewActor) const;
};
