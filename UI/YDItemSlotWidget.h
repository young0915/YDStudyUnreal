// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cores/YDDefine.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "YDItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class YD_API UYDItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> QuantityText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> Equipped;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EYDItemType AcceptItemType;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EYDItemType ItemType = EYDItemType::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemAmount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsEquipped = false;
public :
	UYDItemSlotWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

public:
	void SetItemInfo(EYDItemType InItemType, UTexture2D* InTexture, int32 InAmount =1, bool InIsEquipped = false);
	void ResetItemInfo();
	
public:
	FORCEINLINE UImage* GetItemImage() const { return ItemImage; }
	FORCEINLINE EYDItemType GetItemType() const { return ItemType; }
	FORCEINLINE int32 GetItemAmount() const { return ItemAmount; }
	
	FORCEINLINE bool GetEquipped() const { return bIsEquipped; }
};
