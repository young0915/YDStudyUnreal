// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/YDItemSlotWidget.h"
#include "Components/Image.h"
#include "Kismet/KismetTextLibrary.h"

UYDItemSlotWidget::UYDItemSlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UYDItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ResetItemInfo();
}

void UYDItemSlotWidget::SetItemInfo(EYDItemType InItemType, UTexture2D* InTexture, int32 InAmount, bool InIsEquipped)
{
	ItemAmount = InAmount;
	QuantityText->SetText(ItemAmount >= 2 ? UKismetTextLibrary::Conv_IntToText(ItemAmount) : FText::GetEmpty());
	
	ItemType = InItemType;

	if (InTexture != nullptr)
	{
		ItemImage->SetBrushFromTexture(InTexture);
		ItemImage->SetVisibility(ESlateVisibility::Visible);
	}
	bIsEquipped = InIsEquipped;
	Equipped ->SetText(bIsEquipped? FText::FromString("E") : FText::GetEmpty());
	Equipped->SetColorAndOpacity(FLinearColor::Yellow);
	SetIsEnabled(true);
}

void UYDItemSlotWidget::ResetItemInfo()
{
	if (AcceptItemType == EYDItemType::None || AcceptItemType == EYDItemType::Equipment)
	{
		QuantityText->SetText(FText::GetEmpty());
	}
	ItemImage->SetVisibility(ESlateVisibility::Hidden);
}
