// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "YDWeaponCollisionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitActor, const FHitResult&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class YD_API UYDWeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHitActor OnHitActor;

protected:
	UPROPERTY(EditAnywhere)
	FName TraceStartSocketName;

	UPROPERTY(EditAnywhere)
	FName TraceEndSocketName;

protected:
	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.0f;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> IgnoredActors;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> WeaponMesh;

	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;

	bool bIsCollisionEnabled = false;
public:	
	UYDWeaponCollisionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void TurnOnCollision();

	void TurnOffCollision();

	void SetWeaponMesh(UPrimitiveComponent* NewMeshComponent);

	void AddIgnoreActor(AActor* NewActor);

	void RemoveIgnoreActor(AActor* NewActor);
protected:
	bool CanHitActor(AActor* NewActor) const;

	void CollisionTrace();

public :
	FORCEINLINE void SetTraceRadius(float NewRadius) { TraceRadius = NewRadius; }
};
