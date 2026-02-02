// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "YDCommentWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class YD_API UYDCommentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> Comment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CommentText;

public:
	UYDCommentWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public :
	virtual void NativeConstruct() override;
	void SetTextInfo(const FText& Text);
};
