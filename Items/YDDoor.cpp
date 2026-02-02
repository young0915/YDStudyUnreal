// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/YDDoor.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Cores/YDDefine.h"

AYDDoor::AYDDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	HingeL = CreateDefaultSubobject<USceneComponent>("HingeL");
	HingeL->SetupAttachment(Root);

	HingeR = CreateDefaultSubobject<USceneComponent>("HingeR");
	HingeR->SetupAttachment(Root);

	MeshL = CreateDefaultSubobject<UStaticMeshComponent>("MeshL");
	MeshL->SetupAttachment(HingeL);
	MeshL->SetMobility(EComponentMobility::Movable);

	MeshR = CreateDefaultSubobject<UStaticMeshComponent>("MeshR");
	MeshR->SetupAttachment(HingeR);
	MeshR->SetMobility(EComponentMobility::Movable);

	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger->SetupAttachment(Root);
	Trigger->SetCollisionProfileName("Trigger");
	Trigger->SetGenerateOverlapEvents(true);
}

void AYDDoor::BeginPlay()
{
	Super::BeginPlay();

	InitYawL = CurrYawL  = TargetYawL = HingeL->GetComponentRotation().Yaw;
	InitYawR = CurrYawR  = TargetYawR =HingeR->GetComponentRotation().Yaw;

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OpenDoor);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ThisClass::CloseDoor);

}

void AYDDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyEqual(CurrYawL, TargetYawL, 0.1f))
	{
		CurrYawL = FMath::FInterpConstantTo(CurrYawL, TargetYawL, DeltaTime,DegreesPerSecond);
		HingeL->SetRelativeRotation(FRotator(0, CurrYawL, 0));
	}

	if (!FMath::IsNearlyEqual(CurrYawR, TargetYawR, 0.1f))
	{
		CurrYawR  = FMath::FInterpConstantTo(CurrYawR, TargetYawR, DeltaTime,DegreesPerSecond);
		HingeR->SetRelativeRotation(FRotator(0, CurrYawR, 0));
	}
	
}

void AYDDoor::OpenDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TargetYawL = InitYawL - 90;
	TargetYawR = InitYawR + 90;
}

void AYDDoor::CloseDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	TargetYawL = InitYawL;
	TargetYawR = InitYawR;
}


