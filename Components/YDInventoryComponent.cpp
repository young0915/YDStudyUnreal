// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/YDInventoryComponent.h"

#include "YDAttributeComponent.h"
#include "Character/YDPlayerCharacter.h"
#include "Datas/YDGameSingleton.h"
#include "Items/YDItemBase.h"
#include "Items/YDItemProjectile.h"
#include "Equipments/YDWeapon.h"
#include "GameFramework/Character.h"


UYDInventoryComponent::UYDInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UYDInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UYDInventoryComponent::AddItem(AYDItemBase* ItemValue, int32 Quantity)
{
	if (ItemValue == nullptr || Quantity <= 0 || ItemValue->GetItemCode() == 0)
	{
		return;
	}

	for (UYDItemObject* Item : Items)
	{
		if (Item->Data.ItemCode == ItemValue->GetItemCode())
		{
			Item->Quantity += Quantity;
			return;
		}
	}

	UYDItemObject* NewItem = NewObject<UYDItemObject>();
	NewItem->Data.ItemCode = ItemValue->GetItemCode();
	NewItem->Data.ItemType = ItemValue->GetItemType();
	NewItem->Texture = ItemValue->GetItemSlotIcon();
	NewItem->Quantity = 1;

	Items.Add(NewItem);
}

void UYDInventoryComponent::AddItem(AYDWeapon* WeaponValue)
{
	if (WeaponValue == nullptr || WeaponValue->GetCombatType() == EYDCombatType::Unarmed)
	{
		return;
	}

	for (UYDItemObject* Item : Items)
	{
		if (Item->Data.ItemCode == WeaponValue->GetItemCode())
		{
			return;
		}
	}

	UYDItemObject* NewItem = NewObject<UYDItemObject>();
	NewItem->Data.ItemCode = WeaponValue->GetItemCode();
	NewItem->Texture = WeaponValue->GetItemSlotIcon();
	NewItem->Data.ItemType = EYDItemType::Equipment;
	NewItem->Quantity = 1;
	if (Items.Num() < 1)
	{
		NewItem->IsEquipped = true;
	}
	WeaponValue->SetItemInfo(NewItem);
	Items.Add(NewItem);
}

void UYDInventoryComponent::RemoveItem(UYDItemObject* NewItemValue)
{
	for (UYDItemObject* Item : Items)
	{
		if (Item->Data.ItemCode == NewItemValue->Data.ItemCode)
		{
			if (Item->Quantity > 1)
			{
				Item->Quantity -= 1;
			}
			return;
		}
	}
}

void UYDInventoryComponent::ClearItems()
{
	if (Items.Num() <= 0)
	{
		return;
	}

	Items.Reset();
}

void UYDInventoryComponent::SetSubItem(UYDItemObject* InSubItem)
{
	SubBagItem = InSubItem;
}

void UYDInventoryComponent::UseSubBagItem()
{
	if (!SubBagItem)
	{
		OnSubItemSlotUpdate.Broadcast();
		return;
	}

	const int32 Index = Items.IndexOfByKey(SubBagItem);
	if (!Items.IsValidIndex(Index))
	{
		SubBagItem = nullptr;
		OnSubItemSlotUpdate.Broadcast();
		return;
	}

	UYDItemObject* Item = Items[Index].Get();
	if (!Item)
	{
		SubBagItem = nullptr;
		OnSubItemSlotUpdate.Broadcast();
		return;
	}

	if (Item->Quantity > 1)
	{
		Item->Quantity -= 1;
		OnSubItemSlotUpdate.Broadcast();
		return;
	}

	Items.RemoveAt(Index);
	SubBagItem = FindNextSubBagItem(Index);
	OnSubItemSlotUpdate.Broadcast();
}

UYDItemObject* UYDInventoryComponent::FindNextSubBagItem(int32 InStartIndex) const
{
	const int32 Num = Items.Num();
	if (Num <= 0)
	{
		return nullptr;
	}

	for (int32 i = 0; i < Num; ++i)
	{
		const int32 Cur = (InStartIndex + i) % Num;
		if (const UYDItemObject* Item = Items[Cur].Get())
		{
			const bool bEquipped = Item->IsEquipped;
			const bool bSubItemType = (Item->Data.ItemType == EYDItemType::Potion
				|| Item->Data.ItemType == EYDItemType::Projectile);

			if (bEquipped && bSubItemType)
			{
				return Items[Cur].Get();
			}
		}
	}
	return nullptr;
}

void UYDInventoryComponent::UsePotion()
{
	if (!SubBagItem || SubBagItem->Data.ItemType != EYDItemType::Potion)
	{
		return;
	}
	int32 ItemCode = SubBagItem->Data.ItemCode;
	int32 Health = UYDGameSingleton::Get().GetItemData(ItemCode).Health;

	if (Health > 0)
	{
		if (UYDAttributeComponent* AttributeComp = GetOwner()->GetComponentByClass<UYDAttributeComponent>())
		{
			AttributeComp->Heal(Health);
		}
	}
}

void UYDInventoryComponent::ThrowProjectile()
{
	AYDPlayerCharacter* OwnerCharacter = Cast<AYDPlayerCharacter>(GetOwner());

	if (!OwnerCharacter)
	{
		return;
	}

	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh)
	{
		return;
	}

	const FName SocketName = TEXT("Hand_R_Projectile");
	if (!Mesh->DoesSocketExist(SocketName))
	{
		return;
	}
	const FTransform SocketTransform = Mesh->GetSocketTransform(SocketName, RTS_World);

	FActorSpawnParameters Parms;
	Parms.Owner = OwnerCharacter;
	Parms.Instigator = Cast<APawn>(OwnerCharacter);
	Parms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AYDItemProjectile* Proj = GetWorld()->SpawnActor<AYDItemProjectile>(ItemBase, SocketTransform, Parms);
	if (!Proj)
	{
		return;
	}

	FVector Direction;

	if (OwnerCharacter->GetTargetingComponent()->GetTargetActor() != nullptr)
	{
		AActor*  Target  =OwnerCharacter->GetTargetingComponent()->GetTargetActor();
		const FVector Start = SocketTransform.GetLocation();
		const FVector TargetLocation = Target->GetActorLocation() + FVector(0, 0, 50.f);
		Direction = (TargetLocation- Start).GetSafeNormal();
	}
	else
	{
		Direction = SocketTransform.GetRotation().GetForwardVector();
	}
	Proj->ThrowProjectile(Direction, 3000.0f);
}
