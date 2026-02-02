// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "YDRotationComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YD_API UYDRotationComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldRotate = false;

public:
	UYDRotationComponent();

public :
	FORCEINLINE void SetTargetActor(TObjectPtr<AActor> InTargetActor) { TargetActor = InTargetActor; }
	FORCEINLINE void TogglehouldRotate(const bool InbRotate) { bShouldRotate = InbRotate; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
