// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "YDItemSlotWidget.h"
#include "YDStatBarWidget.h"
#include "Components/YDInventoryComponent.h"
#include "Cores/YDDefine.h"
#include "YDPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class YD_API UYDPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UYDStatBarWidget> HealthBarWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UYDStatBarWidget> StaminaBarWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UYDItemSlotWidget> ItemSlotWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UYDItemSlotWidget> WeaponWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BlankIcon;

protected:
	UPROPERTY()
	TArray<TObjectPtr<UYDItemObject>> CachedSubItems;
	int32 CurrentSubItemIndex = 0;
	
public:
	UYDPlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public :
	virtual void NativeConstruct() override;

protected:
	void OnAttributeChanged(EYDAttributeType InAttributeType, float InValue);

	void OnSubItemSlotUpdate();
	void RebuildCachedSubItems();
	void ShowCurrentSubItem();
	
	void OnWeaponChanged();

public:
	void AdvanceSubItemIndex();

};
