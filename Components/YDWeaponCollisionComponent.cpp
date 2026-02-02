// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/YDWeaponCollisionComponent.h"

UYDWeaponCollisionComponent::UYDWeaponCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}


void UYDWeaponCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UYDWeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}
}

void UYDWeaponCollisionComponent::TurnOnCollision()
{
	AlreadyHitActors.Empty();
	bIsCollisionEnabled = true;
}

void UYDWeaponCollisionComponent::TurnOffCollision()
{
	bIsCollisionEnabled = false;
}

void UYDWeaponCollisionComponent::SetWeaponMesh(UPrimitiveComponent* NewMeshComponent)
{
	WeaponMesh = NewMeshComponent;
}

void UYDWeaponCollisionComponent::AddIgnoreActor(AActor* NewActor)
{
	IgnoredActors.Add(NewActor);
}

void UYDWeaponCollisionComponent::RemoveIgnoreActor(AActor* NewActor)
{
	IgnoredActors.Remove(NewActor);
}


bool UYDWeaponCollisionComponent::CanHitActor(AActor* NewActor) const
{
	return AlreadyHitActors.Contains(NewActor) == false;
}

void UYDWeaponCollisionComponent::CollisionTrace()
{
	TArray<FHitResult> OutHits;

	const FVector Start = WeaponMesh->GetSocketLocation(TraceStartSocketName);
	const FVector End = WeaponMesh->GetSocketLocation(TraceEndSocketName);

	bool const bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TraceRadius,
		TraceObjectTypes,
		false,
		IgnoredActors,
		DrawDebugType,
		OutHits,
		true);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();

			if (HitActor == nullptr)
			{
				continue;
			}

			if (CanHitActor(HitActor))
			{
				AlreadyHitActors.Add(HitActor);

				if (OnHitActor.IsBound())
				{
					OnHitActor.Broadcast(Hit);
				}
			}
		}
	}
}
