// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Cores/YDDefine.h"
#include "YDInventoryComponent.generated.h"

class AYDWeapon;
class AYDItemBase;


USTRUCT(BlueprintType)
struct FYDItemData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EYDItemType ItemType = EYDItemType::None;
};

UCLASS(BlueprintType)
class UYDItemObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FYDItemData Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsEquipped = false;
};

DECLARE_MULTICAST_DELEGATE(FOnSubItemSlotUpdate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YD_API UYDInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnSubItemSlotUpdate OnSubItemSlotUpdate;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory | Item List")
	TArray<TObjectPtr<UYDItemObject>> Items;

protected:
	// Only Potion, Only Projectile
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory | Item List")
	TObjectPtr<UYDItemObject> SubBagItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory | Projectile")
	TSubclassOf<class AYDItemBase> ItemBase;

public:
	UYDInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	void AddItem(AYDItemBase* ItemValue, int32 Quantity);
	void AddItem(AYDWeapon* WeaponValue);
	void RemoveItem(UYDItemObject* NewItemValue);
	void ClearItems();
	void SetSubItem(UYDItemObject* InSubItem);

	FORCEINLINE UYDItemObject* GetItemByIndex(int32 Index)
	{
		return Items.IsValidIndex(Index) ? Items[Index].Get() : nullptr;
	}

	// Sub Bag Section
public:
	void UseSubBagItem();
	UYDItemObject* FindNextSubBagItem(int32 InStartIndex) const;

	// Use Item Action Section
public:
	void UsePotion();
	void ThrowProjectile();

public:
	FORCEINLINE TArray<TObjectPtr<UYDItemObject>> GetInventoryItems() const { return Items; }
	FORCEINLINE UYDItemObject* GetSubBagItem() const { return SubBagItem; }
};
