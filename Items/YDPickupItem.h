// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Equipments/YDEquipment.h"
#include "GameFramework/Actor.h"
#include "Interfaces/YDInteract.h"
#include "Items/YDItemBase.h"
#include "YDPickupItem.generated.h"

UCLASS()
class YD_API AYDPickupItem : public AActor, public IYDInteract
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> CommentWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UYDCommentWidget> CommentWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionComponent;
	
public:	
	AYDPickupItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual  void OnConstruction(const FTransform& Transform) override;
	virtual void Interact(AActor* InteractionActor) override;
	
	void SetCommentWidget(bool bIsActive);

protected:
	UFUNCTION()
	void ActiveComment(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void DeactivateComment(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FORCEINLINE void SetEquipmentClass(const TSubclassOf<AYDEquipment>& NewEquipmentClass) { EquipmentClass = NewEquipmentClass; }
	FORCEINLINE EYDItemType GetItemType() const {return ItemTyp;}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AYDEquipment> EquipmentClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<TSubclassOf<AYDItemBase>> ItemClasses;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EYDItemType ItemTyp = EYDItemType::None;

};


