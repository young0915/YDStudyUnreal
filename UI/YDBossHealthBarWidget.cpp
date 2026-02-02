// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/YDBossHealthBarWidget.h"

void UYDBossHealthBarWidget::Init(UYDAttributeComponent* InAttributeComponent)
{
	if (InAttributeComponent)
	{
		InAttributeComponent->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChanged);
		InAttributeComponent->BroadcastAttributeChanged(EYDAttributeType::Health);
	}
}

void UYDBossHealthBarWidget::OnAttributeChanged(EYDAttributeType AttributeType, float InValue)
{
	if (AttributeType == EYDAttributeType::Health)
	{
		HealthBarWidget->SetRatio(InValue);
	}
}
