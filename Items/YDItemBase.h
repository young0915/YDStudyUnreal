// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cores/YDDefine.h"
#include "Components/YDInventoryComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "YDItemBase.generated.h"


UCLASS()
class YD_API AYDItemBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	EYDItemType ItemType = EYDItemType::None;

	UPROPERTY(EditAnywhere, Category= "Item")
	TObjectPtr<UStaticMesh> MeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	int32 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UTexture2D> ItemIcon;

protected:
	UPROPERTY()
	TObjectPtr<UYDInventoryComponent> InventoryComponent;



public:
	AYDItemBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

public :
	void PickUpItem();

public:
	FORCEINLINE EYDItemType GetItemType() const { return ItemType; }
	FORCEINLINE int32 GetItemCode() const { return ItemCode; }
	FORCEINLINE UTexture2D* GetItemSlotIcon() const { return ItemIcon; }
};
