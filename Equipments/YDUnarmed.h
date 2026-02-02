// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/YDWeapon.h"
#include "YDUnarmed.generated.h"

/**
 * 
 */
UCLASS()
class YD_API AYDUnarmed : public AYDWeapon
{
	GENERATED_BODY()

public:
	AYDUnarmed();

public:
	virtual void EquipItem() override;
};
