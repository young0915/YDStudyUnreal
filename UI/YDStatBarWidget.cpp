// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/YDStatBarWidget.h"

UYDStatBarWidget::UYDStatBarWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void UYDStatBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (StatBar)
	{
		if (StatFillTexture)
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(StatFillTexture);
			Brush.ImageSize = FVector2D(StatFillTexture->GetSizeX(), StatFillTexture->GetSizeY());
			Brush.DrawAs = ESlateBrushDrawType::Image;

			StatBar->WidgetStyle.FillImage = Brush;
			StatBar->SynchronizeProperties();
		}
	}
}

void UYDStatBarWidget::SetRatio(float Ratio) const
{
	if (StatBar)
	{
		StatBar->SetPercent(Ratio);
	}
}
