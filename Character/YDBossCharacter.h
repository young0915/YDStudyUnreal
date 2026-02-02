// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/YDEnemyCharacter.h"
#include "UI/YDBossHealthBarWidget.h"

#include "YDBossCharacter.generated.h"

/**
 * 
 */
UCLASS()
class YD_API AYDBossCharacter : public AYDEnemyCharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> BossHealtBarWidgetClass;

	UPROPERTY()
	TObjectPtr<UYDBossHealthBarWidget> BossHealthBarWidget;

protected:
	bool bStarteBossMusic = false;

public:
	virtual void SeesTarget(AActor* InTargetAcor) override;
	virtual void Tick(float InDeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnDeath() override;

};
