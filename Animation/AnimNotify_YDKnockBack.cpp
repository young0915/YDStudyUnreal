// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_YDKnockBack.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotify_YDKnockBack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	const FVector OriginLocation = MeshComp->GetSocketLocation(LocationSocketName);

	APawn* OwnerPawn = Cast<APawn>(MeshComp->GetOwner());
	if (!OwnerPawn)
	{
		return;
	}

	AController* OwnerController = OwnerPawn->GetController();
	if (!OwnerController)
	{
		return;
	}

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerPawn);

	UGameplayStatics::ApplyRadialDamage(OwnerPawn, BaseDamage, OriginLocation, DamageRadius,nullptr,
		IgnoreActors,nullptr, OwnerController, false,
		ECC_Visibility);
}
