// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/YDPlayerHUDWidget.h"
#include "Components/YDAttributeComponent.h"
#include "Components/YDCombatComponent.h"
#include "Components/YDInventoryComponent.h"
#include "Equipments/YDWeapon.h"

UYDPlayerHUDWidget::UYDPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UYDPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwingPawn = GetOwningPlayerPawn())
	{
		if (UYDAttributeComponent* Attribute = OwingPawn->GetComponentByClass<UYDAttributeComponent>())
		{
			Attribute->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChanged);
			Attribute->BroadcastAttributeChanged(EYDAttributeType::Stamina);
			Attribute->BroadcastAttributeChanged(EYDAttributeType::Health);
		}

		if (UYDCombatComponent* CombatComponent = OwingPawn->GetComponentByClass<UYDCombatComponent>())
		{
			CombatComponent->OnChangedWeapon.AddUObject(this, &ThisClass::OnWeaponChanged);
			CombatComponent->OnChangedWeapon.Broadcast();
		}
		
		if (UYDInventoryComponent* InventoryComponent = OwingPawn->GetComponentByClass<UYDInventoryComponent>())
		{
			InventoryComponent->OnSubItemSlotUpdate.AddUObject(this, &ThisClass::OnSubItemSlotUpdate);
			InventoryComponent->OnSubItemSlotUpdate.Broadcast();
		}
	}
}

void UYDPlayerHUDWidget::OnAttributeChanged(EYDAttributeType InAttributeType, float InValue)
{
	switch (InAttributeType)
	{
	case EYDAttributeType::Health:
		HealthBarWidget->SetRatio(InValue);
		break;
	case EYDAttributeType::Stamina:
		StaminaBarWidget->SetRatio(InValue);
		break;
	}
}

void UYDPlayerHUDWidget::OnSubItemSlotUpdate()
{
	RebuildCachedSubItems();
	ShowCurrentSubItem();
}

void UYDPlayerHUDWidget::RebuildCachedSubItems()
{
	CachedSubItems.Reset();

	const APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn)
	{
		return;
	}

	if (UYDInventoryComponent* InventoryComp = Pawn->GetComponentByClass<UYDInventoryComponent>())
	{
		Algo::CopyIf(
			InventoryComp->GetInventoryItems(),
			CachedSubItems,
			[](const TObjectPtr<UYDItemObject>& It)
			{
				return It && It->IsEquipped &&(It->Data.ItemType == EYDItemType::Potion ||
					It->Data.ItemType == EYDItemType::Projectile);
			});
	}

	if (CachedSubItems.IsEmpty())
	{
		CurrentSubItemIndex = 0;
	}
	else
	{
		CurrentSubItemIndex %= CachedSubItems.Num();
	}
}

void UYDPlayerHUDWidget::ShowCurrentSubItem()
{
	if (!ItemSlotWidget)
	{
		return;
	}

	if (CachedSubItems.IsEmpty())
	{
		ItemSlotWidget->ResetItemInfo();
		return;
	}

	if (UYDItemObject* Item = CachedSubItems[CurrentSubItemIndex].Get())
	{
		ItemSlotWidget->SetItemInfo(Item->Data.ItemType, Item->Texture, Item->Quantity);

		if (APawn* Pawn = GetOwningPlayerPawn())
		{
			if (UYDInventoryComponent* InventoryComp = Pawn->GetComponentByClass<UYDInventoryComponent>())
			{
				InventoryComp->SetSubItem(Item);
			}
		}
	}
	else
	{
		ItemSlotWidget->ResetItemInfo();
	}
}

void UYDPlayerHUDWidget::AdvanceSubItemIndex()
{
	if (CachedSubItems.IsEmpty())
	{
		ShowCurrentSubItem();
		return;
	}

	CurrentSubItemIndex = (CurrentSubItemIndex + 1) % CachedSubItems.Num();
	ShowCurrentSubItem();
}


void UYDPlayerHUDWidget::OnWeaponChanged()
{
	if (const APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (const UYDCombatComponent* CombatComponent = OwningPawn->GetComponentByClass<UYDCombatComponent>())
		{
			UTexture2D* WeaponIconTexture = BlankIcon;

			if (const AYDWeapon* MainWeapon = CombatComponent->GetMainWeapon())
			{
				if (MainWeapon->GetCombatType() != EYDCombatType::Unarmed)
				{
					WeaponIconTexture = MainWeapon->GetEquipmentIcon();
				}

				if (::IsValid(WeaponIconTexture))
				{
					WeaponWidget->SetItemInfo(EYDItemType::Equipment, WeaponIconTexture);
				}
			}
		}
	}
}
