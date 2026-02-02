// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YDInventoryEntry.h"
#include "YDItemSlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/YDInventoryComponent.h"
#include "YDInventoryWidget.generated.h"


UCLASS()
class YD_API UYDInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> Weapons;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> Potions;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> Projectiles;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DetailsText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BlankIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EquippedButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> UnequippedButton;

	UPROPERTY()
	UYDItemObject* SelectedItem;
	
public :
	UYDInventoryWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public :
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void RefreshList();

	UFUNCTION()
	void HandleListItemClicked(UObject* ClickedItem);

	void ShowItemDetails(class UYDItemObject* Data);
	void ClearItemDetails();

	UFUNCTION(BlueprintCallable)
	void RefreshButton();

	UFUNCTION()
	void OnclickedEquip();

	UFUNCTION()
	void OnClickedUnequip();

private :
	void RefreshListEntryForItem(UListView* InList, UYDItemObject* InItemObject, bool InIsEquip);

	UYDInventoryEntry* GetSelectedEntry(UYDItemObject* InItemObject) const;
};
