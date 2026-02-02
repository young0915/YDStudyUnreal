// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/YDInventoryEntry.h"
#include "Components/YDInventoryComponent.h"

UYDInventoryEntry::UYDInventoryEntry(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer){}

void UYDInventoryEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UYDInventoryEntry::SetItemInfo(EYDItemType InItemType, UTexture2D* InTexture, int32 InAmount, bool InIsEquipped)
{
	if (ItemSlot)
	{
		ItemSlot->SetItemInfo(InItemType,InTexture, InAmount, InIsEquipped);
	}
}


void UYDInventoryEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	if (UYDItemObject* Item = Cast<UYDItemObject>(ListItemObject))
	{
		SetItemInfo(Item->Data.ItemType, Item->Texture, Item->Quantity ,Item->IsEquipped);
	}
}
