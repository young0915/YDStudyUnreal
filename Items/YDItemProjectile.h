// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Items/YDItemBase.h"
#include "YDItemProjectile.generated.h"

/**
 * 
 */
UCLASS()
class YD_API AYDItemProjectile : public AYDItemBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> DamageSphere;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ExplosionFx;

	UPROPERTY()
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	float ExplosionRadius = 300.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float ExplosionDamage =40.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> DamageTypeClass;

	FTimerHandle DestroyTimer;
	FTimerHandle DamageTimerHandle;
	
public:
	AYDItemProjectile();
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;


public:
	void ThrowProjectile(const FVector& Dir, float Speed);
	UFUNCTION()
	void EndDamageWindow();

private :
	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
	void Explode(const FHitResult& Hit);

};
