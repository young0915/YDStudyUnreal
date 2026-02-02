// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cores/YDDefine.h"
#include "GameFramework/Actor.h"
#include "YDEquipment.generated.h"

UCLASS()
class YD_API AYDEquipment : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category= "Equipment | Mesh")
	TObjectPtr<UStaticMesh> MeshAsset;

	UPROPERTY(EditAnywhere, Category= "Equipment | Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Icon")
	TObjectPtr<UTexture2D> WeaponIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Icon")
	TObjectPtr<UTexture2D> ItemslotIcon;
	
	UPROPERTY(EditAnywhere, Category= "Equipment | Mesh")
	int32 ItemCode;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	EYDCombatType CombatType = EYDCombatType::Unarmed;
	
	UPROPERTY()
	bool bIsActive = false;

	UPROPERTY()
	bool bIsLoaded = false;

public:
	AYDEquipment();

protected:
	virtual auto BeginPlay() -> void override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void CreateItem();
	virtual void EquipItem();

	virtual void UnEquipItem();

	virtual void AttachToOwner(FName SocketName);

protected:
	void SetActiveEquipment(bool InbIsActive);

public:
	FORCEINLINE EYDCombatType GetCombatType() const { return CombatType; }
	FORCEINLINE bool GetIsActive() const { return bIsActive; }
	FORCEINLINE UTexture2D* GetEquipmentIcon() const { return WeaponIcon; }
	FORCEINLINE UTexture2D* GetItemSlotIcon() const { return ItemslotIcon; }
	FORCEINLINE int32 GetItemCode() const { return ItemCode; } 
};
