// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "YDStatBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class YD_API UYDStatBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UProgressBar> StatBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "StatFillImage")
	TObjectPtr<UTexture2D> StatFillTexture;

public:
	UYDStatBarWidget(const FObjectInitializer& ObjectInitializer= FObjectInitializer::Get());

public:
	virtual void NativeConstruct() override;

public:
	void SetRatio(float Ratio) const;
};
