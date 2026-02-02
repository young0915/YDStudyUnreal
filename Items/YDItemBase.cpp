// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/YDItemBase.h"

AYDItemBase::AYDItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AYDItemBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshAsset)
	{
		Mesh->SetStaticMesh(MeshAsset);
	}
}

void AYDItemBase::PickUpItem()
{
	InventoryComponent = GetOwner()->GetComponentByClass<UYDInventoryComponent>();
	if (InventoryComponent)
	{
		InventoryComponent->AddItem(this , 1);
	}

	SetLifeSpan(2.0f);
}
