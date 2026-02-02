// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/YDInventoryWidget.h"

#include "YDInventoryEntry.h"
#include "Character/YDPlayerCharacter.h"
#include "Components/YDInventoryComponent.h"
#include "Datas/YDGameSingleton.h"

UYDInventoryWidget::UYDInventoryWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UYDInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (EquippedButton)
	{
		EquippedButton->OnClicked.AddDynamic(this, &ThisClass::OnclickedEquip);
	}

	if (UnequippedButton)
	{
		UnequippedButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedUnequip);
	}
}

void UYDInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Weapons || Potions || Projectiles)
	{
		RefreshList();
	}

	if (Weapons)
	{
		Weapons->SetSelectionMode(ESelectionMode::Single);
		Weapons->OnItemClicked().AddUObject(this, &ThisClass::HandleListItemClicked);
	}

	if (Potions)
	{
		Potions->SetSelectionMode(ESelectionMode::Single);
		Potions->OnItemClicked().AddUObject(this, &ThisClass::HandleListItemClicked);
	}

	if (Projectiles)
	{
		Projectiles->SetSelectionMode(ESelectionMode::Single);
		Projectiles->OnItemClicked().AddUObject(this, &ThisClass::HandleListItemClicked);
	}

	ClearItemDetails();
	RefreshButton();
}

void UYDInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UYDInventoryWidget::RefreshList()
{
	int32 SlotCount = 4;
	if (APawn* OwnerPawn = GetOwningPlayerPawn())
	{
		if (UYDInventoryComponent* InventoryComp = OwnerPawn->GetComponentByClass<UYDInventoryComponent>())
		{
			const TArray<TObjectPtr<UYDItemObject>>& Src = InventoryComp->GetInventoryItems();

			TArray<UObject*> WeaponsObjs;
			WeaponsObjs.Reset(SlotCount);

			TArray<UObject*> PotionsObjs;
			PotionsObjs.Reset(SlotCount);

			TArray<UObject*> ProjectileObjs;
			ProjectileObjs.Reset(SlotCount);


			auto PushItem = [&](TArray<UObject*>& Value, const UYDItemObject* ItemValue)
			{
				if (Value.Num() >= SlotCount) return;

				UYDItemObject* UI = NewObject<UYDItemObject>(this);
				UI->Quantity = ItemValue->Quantity;
				UI->Texture = ItemValue->Texture;
				UI->Data.ItemCode = ItemValue->Data.ItemCode;
				UI->Data.ItemType = ItemValue->Data.ItemType;
				UI->IsEquipped = ItemValue->IsEquipped;
				Value.Add(UI);
			};

			for (const TObjectPtr<UYDItemObject>& Item : Src)
			{
				if (!IsValid(Item))
				{
					continue;
				}

				const UYDItemObject* ItemObject = Item.Get();

				switch (ItemObject->Data.ItemType)
				{
				case EYDItemType::Equipment:
					PushItem(WeaponsObjs, ItemObject);
					break;

				case EYDItemType::Potion:
					PushItem(PotionsObjs, ItemObject);
					break;

				case EYDItemType::Projectile:
					PushItem(ProjectileObjs, ItemObject);
					break;
				}
			}

			auto PadToFour = [&](TArray<UObject*>& Arr)
			{
				while (Arr.Num() < SlotCount)
				{
					UYDItemObject* EmptyItem = NewObject<UYDItemObject>(this);
					EmptyItem->Quantity = 0;
					EmptyItem->Texture = BlankIcon;
					EmptyItem->Data = EmptyItem->Data;
					EmptyItem->IsEquipped = false;
					Arr.Add(EmptyItem);
				}
			};

			PadToFour(WeaponsObjs);
			PadToFour(PotionsObjs);
			PadToFour(ProjectileObjs);

			Weapons->SetListItems(WeaponsObjs);
			Potions->SetListItems(PotionsObjs);
			Projectiles->SetListItems(ProjectileObjs);
		}
	}
}

void UYDInventoryWidget::HandleListItemClicked(UObject* ClickedItem)
{
	UYDItemObject* ItemData = Cast<UYDItemObject>(ClickedItem);

	if (!ItemData)
	{
		return;
	}

	ShowItemDetails(ItemData);
}

void UYDInventoryWidget::ShowItemDetails(class UYDItemObject* Data)
{
	if (Data == nullptr || Data->Data.ItemCode <= 0)
	{
		return;
	}

	SelectedItem = Data;
	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(Data->Texture);
		ItemImage->SetVisibility(ESlateVisibility::Visible);
		ItemImage->SetColorAndOpacity(FLinearColor::White);
	}

	if (DetailsText)
	{
		int32 ItemCode = Data->Data.ItemCode;

		FString ItemName = UYDGameSingleton::Get().GetItemData(ItemCode).ItemName;
		int32 Attack = UYDGameSingleton::Get().GetItemData(ItemCode).Attack;
		int32 Defense = UYDGameSingleton::Get().GetItemData(ItemCode).Defense;
		int32 Health = UYDGameSingleton::Get().GetItemData(ItemCode).Health;

		FString DetailString;

		if (Data->Data.ItemType == EYDItemType::Equipment)
		{
			DetailString = FString::Printf(TEXT("%s \n ATK : %d \n DEF : %d \n"), *ItemName, Attack, Defense);
		}
		else if (Data->Data.ItemType == EYDItemType::Projectile)
		{
			DetailString = FString::Printf(TEXT("%s \n ATK : %d "), *ItemName, Attack);
		}
		else if (Data->Data.ItemType == EYDItemType::Potion)
		{
			DetailString = FString::Printf(TEXT("%s \n HP : %d"), *ItemName, Health);
		}

		DetailsText->SetText(FText::FromString(*DetailString));
	}
	RefreshButton();
}

void UYDInventoryWidget::ClearItemDetails()
{
	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(BlankIcon);
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
	}

	if (DetailsText)
	{
		DetailsText->SetText(FText::FromString(""));
	}

	SelectedItem = nullptr;
	RefreshButton();
}

void UYDInventoryWidget::RefreshButton()
{
	if (SelectedItem == nullptr)
	{
		if (EquippedButton)
		{
			EquippedButton->SetIsEnabled(false);
			EquippedButton->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (UnequippedButton)
		{
			UnequippedButton->SetIsEnabled(false);
			UnequippedButton->SetVisibility(ESlateVisibility::Collapsed);
		}
		return;
	}

	const bool bEquipped = SelectedItem->IsEquipped;
	if (UnequippedButton)
	{
		UnequippedButton->SetIsEnabled(bEquipped);
		UnequippedButton->SetVisibility(bEquipped ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (EquippedButton)
	{
		EquippedButton->SetIsEnabled(!bEquipped);
		EquippedButton->SetVisibility(bEquipped ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}

void UYDInventoryWidget::OnclickedEquip()
{
	if (!SelectedItem || SelectedItem->IsEquipped)
	{
		return;
	}

	if (SelectedItem->Data.ItemType == EYDItemType::Projectile)
	{
		RefreshListEntryForItem(Projectiles, SelectedItem, true);
	}
	else if (SelectedItem->Data.ItemType == EYDItemType::Potion)
	{
		RefreshListEntryForItem(Potions, SelectedItem, true);
	}
	else if (SelectedItem->Data.ItemType == EYDItemType::Equipment)
	{
		RefreshListEntryForItem(Weapons, SelectedItem, true);
	}

	RefreshButton();
}

void UYDInventoryWidget::OnClickedUnequip()
{
	if (!SelectedItem || !SelectedItem->IsEquipped)
	{
		return;
	}

	if (SelectedItem->Data.ItemType == EYDItemType::Projectile)
	{
		RefreshListEntryForItem(Projectiles, SelectedItem, false);
	}
	else if (SelectedItem->Data.ItemType == EYDItemType::Potion)
	{
		RefreshListEntryForItem(Potions, SelectedItem, false);
	}
	else if (SelectedItem->Data.ItemType == EYDItemType::Equipment)
	{
		RefreshListEntryForItem(Weapons, SelectedItem, false);
	}
	

	RefreshButton();
}


void UYDInventoryWidget::RefreshListEntryForItem(UListView* InList, UYDItemObject* InItemObject, bool InIsEquip)
{
	APawn* OwnerPawn = GetOwningPlayerPawn();
	if (!OwnerPawn)
	{
		return;
	}

	UYDInventoryComponent* InventoryComp = OwnerPawn->GetComponentByClass<UYDInventoryComponent>();
	if (!InventoryComp)
	{
		return;
	}

	if (UUserWidget* EntryWidget = InList->GetEntryWidgetFromItem(InItemObject))
	{
		if (auto* Entry = Cast<UYDInventoryEntry>(EntryWidget))
		{
			Entry->SetItemInfo(InItemObject->Data.ItemType, InItemObject->Texture, InItemObject->Quantity,
			                   InIsEquip);

			for (UYDItemObject* item : InventoryComp->GetInventoryItems())
			{
				if (item->Data.ItemCode == InItemObject->Data.ItemCode)
				{
					item->IsEquipped = InIsEquip;
					ShowItemDetails(item);
					break;
				}
			}
		}
	}

	RefreshList();

	if (InItemObject->Data.ItemType == EYDItemType::Equipment)
	{
		if (UYDCombatComponent* CombatComp = OwnerPawn->GetComponentByClass<UYDCombatComponent>())
		{
			CombatComp->EquipWeapon(InItemObject->Data.ItemCode, InIsEquip);
		}
	}
	else
	{
		if (InventoryComp->OnSubItemSlotUpdate.IsBound())
		{
			InventoryComp->OnSubItemSlotUpdate.Broadcast();
		}
	}
}
