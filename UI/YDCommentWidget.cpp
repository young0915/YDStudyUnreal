// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/YDCommentWidget.h"
#include "Components/TextBlock.h"

UYDCommentWidget::UYDCommentWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UYDCommentWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Comment->SetText(CommentText);
}

void UYDCommentWidget::SetTextInfo(const FText& Text)
{
	CommentText = Text;
	Comment->SetText(CommentText);
}

