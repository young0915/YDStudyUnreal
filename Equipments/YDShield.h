// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/YDEquipment.h"
#include "YDShield.generated.h"

/**
 * 
 */
UCLASS()
class YD_API AYDShield : public AYDEquipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EquipSocketName;

public:
	virtual void EquipItem() override;
	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
};
