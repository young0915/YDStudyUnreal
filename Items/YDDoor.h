// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/YDInteract.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "YDDoor.generated.h"

class UWidgetComponent;

UCLASS()
class YD_API AYDDoor : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> HingeL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> HingeR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Trigger;
private:
	float InitYawL=0;
	float InitYawR=0;
	float CurrYawL=0;
	float CurrYawR=0;
	float TargetYawL=0;
	float TargetYawR=0;
	
	UPROPERTY(EditAnywhere)
	float DegreesPerSecond = 90.0f;
	
public:	
	AYDDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
protected:

	UFUNCTION()
	void OpenDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CloseDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
