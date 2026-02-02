// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/YDRotationComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "ThumbnailRendering/WorldThumbnailInfo.h"

UYDRotationComponent::UYDRotationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UYDRotationComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UYDRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TargetActor)
	{
		return;
	}

	if (!bShouldRotate)
	{
		return;
	}
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());

		GetOwner()->SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
}

