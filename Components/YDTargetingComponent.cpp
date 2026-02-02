// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/YDTargetingComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/YDTargeting.h"
#include "Kismet/KismetMathLibrary.h"

UYDTargetingComponent::UYDTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UYDTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Camera = Character->GetComponentByClass<UCameraComponent>();
	}
}


void UYDTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsLockOn == false)
	{
		return;
	}
	
	if (IsValid(Character) == false)
	{
		return;
	}
	
	if (IsValid(LockedTargetActor) == false)
	{
		return;
	}
	
	const float Distance = FVector::Distance(Character->GetActorLocation(), LockedTargetActor->GetActorLocation());
	
	if (IYDTargeting* Targeting = Cast<IYDTargeting>(GetOwner()))
	{
		if (Targeting->CanBeTargeted() == false || Distance > TargetingRadius)
		{
			StopLockOn();
		}
		else
		{
			FaceLockOnActor();
		}
	}

}

void UYDTargetingComponent::ToggleLockOn()
{
	if (bIsLockOn)
	{
		StopLockOn();
	}
	else
	{
		LockOnTarget();
	}
}

void UYDTargetingComponent::SwitchingLockedOnActor(ESwitchingDirection NewDirection)
{
	if (::IsValid(LockedTargetActor))
	{
		if (IYDTargeting* Targeting = Cast<IYDTargeting>(LockedTargetActor))
		{
			Targeting->OnTargeted(false);
	
			TArray<AActor*> OutTargets;
			FindTargets(OutTargets);
			AActor* TargetActor = FindClosestTarget(OutTargets, NewDirection);
	
			if (::IsValid(TargetActor))
			{
				if (IYDTargeting* NewTargeting = Cast<IYDTargeting>(TargetActor))
				{
					LockedTargetActor = TargetActor;
					NewTargeting->OnTargeted(true);
				}
			}
			else
			{
				StopLockOn();
			}
		}
	}
}

AActor* UYDTargetingComponent::GetTargetActor() const
{
	return (bIsLockOn &&  LockedTargetActor!= nullptr) ? LockedTargetActor : nullptr;
}

void UYDTargetingComponent::FindTargets(TArray<AActor*>& OutTargetsActors) const
{
	TArray<FHitResult> OutHits;
	const FVector Start = Character->GetActorLocation();
	const FVector End = Start;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_TARGETING));

	TArray<AActor*> ActorsToIgnore;

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TargetingRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		OutHits,
		true);

	if (bHit)
	{
		for (const FHitResult& Result : OutHits)
		{
			AActor* HitActor = Result.GetActor();
			if (IYDTargeting* Targeting = Cast<IYDTargeting>(HitActor))
			{
				if (Targeting->CanBeTargeted())
				{
					OutTargetsActors.Add(HitActor);
				}
			}
		}
	}
	
}

AActor* UYDTargetingComponent::FindClosestTarget(TArray<AActor*>& InTargetsActors, ESwitchingDirection NewDirection)
{
	float TargetCompareValue = 0.f;
	AActor* ClosestTarget = nullptr;
	
	for (const AActor* TargetActor : InTargetsActors)
	{
		if (NewDirection != ESwitchingDirection::None && LockedTargetActor == TargetActor)
		{
			continue;
		}
	
		FHitResult OutHit;
		const FVector Start = Camera->GetComponentLocation();
		const FVector End = TargetActor->GetActorLocation();
	
		TArray<AActor*> ActorsToIgnore;
	
		const bool bHit = UKismetSystemLibrary::LineTraceSingle(
			GetOwner(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			DrawDebugType,
			OutHit,
			true);
	
		if (bHit)
		{
			if (NewDirection == ESwitchingDirection::Left)
			{
				if (FVector::DotProduct(Camera->GetRightVector(), OutHit.Normal) > 0.f == false)
				{
					continue;
				}
			}
	
			if (NewDirection == ESwitchingDirection::Right)
			{
				if (FVector::DotProduct(Camera->GetRightVector(), OutHit.Normal) < 0.f == false)
				{
					continue;
				}
			}
	
			AActor* HitActor = OutHit.GetActor();
	
			const FRotator LockAtRotation = UKismetMathLibrary::FindLookAtRotation(
				Character->GetActorLocation(), HitActor->GetActorLocation());
			float CheckValue = FVector::DotProduct(Camera->GetForwardVector(), LockAtRotation.Vector());
	
			if (CheckValue > TargetCompareValue)
			{
				TargetCompareValue = CheckValue;
				ClosestTarget = HitActor;
			}
		}
	}

	return ClosestTarget;
}

void UYDTargetingComponent::OrientCamera() const
{
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void UYDTargetingComponent::OrientMovement() const
{
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UYDTargetingComponent::FaceLockOnActor() const
{
	const FRotator CurrentControlRotation = Character->GetControlRotation();

	const FVector TargetLocation = LockedTargetActor->GetActorLocation() - FVector(0.0f, 0.0f, 150.f);
	const FRotator TargetLookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		Character->GetActorLocation(), TargetLocation);

	FRotator InterpRotation = FMath::RInterpTo(CurrentControlRotation, TargetLookAtRotation,
	                                           GetWorld()->GetDeltaSeconds(), FaceLockOnRotationSpeed);
	Character->GetController()->SetControlRotation(FRotator(InterpRotation.Pitch, InterpRotation.Yaw,
	                                                        InterpRotation.Roll));
}

void UYDTargetingComponent::LockOnTarget()
{
	TArray<AActor*> OutTargets;
	FindTargets(OutTargets);
	AActor* TargetActor = FindClosestTarget(OutTargets);
	
	if (::IsValid(TargetActor))
	{
		LockedTargetActor = TargetActor;
		bIsLockOn = true;
		if (IYDTargeting* Targeting = Cast<IYDTargeting>(LockedTargetActor))
		{
			Targeting->OnTargeted(bIsLockOn);
		}
	
		OrientCamera();
	}

}

void UYDTargetingComponent::StopLockOn()
{
	bIsLockOn = false;
	
	if (IYDTargeting* Targeting = Cast<IYDTargeting>(LockedTargetActor))
	{
		Targeting->OnTargeted(bIsLockOn);
	}
	LockedTargetActor = nullptr;
	OrientMovement();

}
