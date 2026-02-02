// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YDStatBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/YDAttributeComponent.h"
#include "YDBossHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class YD_API UYDBossHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta =(BindWidget), BlueprintReadWrite)
	TObjectPtr<UYDStatBarWidget> HealthBarWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> BossName;

public:
	void Init(UYDAttributeComponent* InAttributeComponent);

protected:
	void OnAttributeChanged(EYDAttributeType AttributeType, float InValue);
};
