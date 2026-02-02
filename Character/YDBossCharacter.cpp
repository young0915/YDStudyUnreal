// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/YDBossCharacter.h"

#include "AIController.h"
#include "Datas/YDGameSingleton.h"
#include "UI/YDBossHealthBarWidget.h"

class AAIController;

void AYDBossCharacter::SeesTarget(AActor* InTargetAcor)
{
	if (IsValid(InTargetAcor))
	{
		if (BossHealthBarWidget)
		{
			BossHealthBarWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	CharacterID = UYDGameSingleton::Get().CharacterStatMaxIndex;

}

void AYDBossCharacter::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
}

void AYDBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->DestroyComponent();
	}

	if (BossHealtBarWidgetClass)
	{
		BossHealthBarWidget= CreateWidget<UYDBossHealthBarWidget>(GetWorld(), BossHealtBarWidgetClass);
		if (BossHealthBarWidget)
		{
			BossHealthBarWidget->Init(AttributeComponent);
			BossHealthBarWidget->AddToViewport();
			BossHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AYDBossCharacter::OnDeath()
{
	Super::OnDeath();

	
	if (BossHealthBarWidget)
	{
		BossHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}

}
