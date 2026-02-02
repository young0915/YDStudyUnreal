// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/YDEquipment.h"
#include"GameFramework/Character.h"

AYDEquipment::AYDEquipment()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AYDEquipment::BeginPlay()
{
	Super::BeginPlay();
}

void AYDEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AYDEquipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshAsset)
	{
		Mesh->SetStaticMesh(MeshAsset);
	}
}

void AYDEquipment::CreateItem()
{
}

void AYDEquipment::EquipItem()
{
	SetActiveEquipment(true);
}

void AYDEquipment::UnEquipItem()
{
	SetActiveEquipment(false);
}


void AYDEquipment::SetActiveEquipment(bool InbIsActive)
{
	SetActorHiddenInGame(!InbIsActive);
	SetActorEnableCollision(InbIsActive);
	SetActorTickEnabled(InbIsActive);

	bIsActive= InbIsActive;
}

void AYDEquipment::AttachToOwner(FName SocketName)
{
	if (ACharacter* OwnerCaharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCaharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true) , SocketName);
		}
	}
}
