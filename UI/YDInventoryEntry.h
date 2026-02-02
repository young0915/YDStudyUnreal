// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include  "Blueprint/IUserObjectListEntry.h"
#include "Components/YDInventoryComponent.h"
#include "UI/YDItemSlotWidget.h"
#include "YDInventoryEntry.generated.h"

class UYDInventoryEntry;
class UYDItemSlotWidget;
/**
 * 
 */
UCLASS()
class YD_API UYDInventoryEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UYDItemSlotWidget> ItemSlot;

public:
	UYDInventoryEntry(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

public:
	void SetItemInfo(EYDItemType InItemType, UTexture2D* InTexture, int32 InAmount, bool InIsEquipped = false);
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
