// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Cores/YDDefine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "YDTargetingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YD_API UYDTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetingRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FaceLockOnRotationSpeed = 20.0f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY()
	TObjectPtr<AActor> LockedTargetActor;

	bool bIsLockOn = false;

public:
	UYDTargetingComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ToggleLockOn();
	void SwitchingLockedOnActor(ESwitchingDirection NewDirection);

	FORCEINLINE bool IsLockOn() const { return bIsLockOn; }

	AActor* GetTargetActor() const;

protected:
	void FindTargets(OUT TArray<AActor*>& OutTargetsActors) const;

	AActor* FindClosestTarget(OUT TArray<AActor*>& InTargetsActors,ESwitchingDirection NewDirection =ESwitchingDirection::None);

	void OrientCamera() const;
	void OrientMovement() const;

	void FaceLockOnActor() const;

	void LockOnTarget();

	void StopLockOn();
};
