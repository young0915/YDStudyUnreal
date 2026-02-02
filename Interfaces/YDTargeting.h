// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "YDTargeting.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UYDTargeting : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class YD_API IYDTargeting
{
	GENERATED_BODY()

public:
	virtual void OnTargeted(bool bTargeted) = 0;
	virtual bool CanBeTargeted() =0;
};
