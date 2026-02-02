// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/YDPickupItem.h"

#include "Cores/YDDefine.h"
#include "Items/YDItemBase.h"
#include "UI/YDCommentWidget.h"

class AYDWeapon;

AYDPickupItem::AYDPickupItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionObjectType(COLLISION_OBJECT_INTERACTION);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CommentWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CommentWidget"));
	CommentWidget->SetupAttachment(GetRootComponent());
	CommentWidget->SetRelativeLocation(FVector(0, 0, 50));
	CommentWidget->SetDrawSize(FVector2D(100, 30));
	CommentWidget->SetWidgetSpace(EWidgetSpace::Screen);
	CommentWidget->SetVisibility(false);

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(GetRootComponent());
	CollisionComponent ->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComponent->SetGenerateOverlapEvents(true);
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AYDPickupItem::ActiveComment);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AYDPickupItem::DeactivateComment);
}

void AYDPickupItem::BeginPlay()
{
	Super::BeginPlay();

	if (CommentWidget)
	{
		if (UYDCommentWidget* Comment = Cast<UYDCommentWidget>(CommentWidget->GetWidget()))
		{
			Comment->SetTextInfo(FText::FromString(TEXT("Interact")));
		}
	
	}
}

void AYDPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AYDPickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (EquipmentClass)
	{
		if (AYDEquipment* CDO = EquipmentClass->GetDefaultObject<AYDEquipment>())
		{
			Mesh->SetStaticMesh(CDO->MeshAsset);
			Mesh->SetSimulatePhysics(true);
		}
	}
}

void AYDPickupItem::Interact(AActor* InteractionActor)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InteractionActor;

	if (ItemTyp == EYDItemType::Potion)
	{
		int32 Index = FMath::RandRange(0, ItemClasses.Num() - 1);

		if (ItemClasses[Index])
		{
			AYDItemBase* AYDItem =GetWorld()->SpawnActor<AYDItemBase>(ItemClasses[Index], GetActorTransform(), SpawnParams);
			if (AYDItem)
			{
				AYDItem->PickUpItem();
				Destroy();
			}
		}
	}
	else
	{
		AYDEquipment* SpawnItem = GetWorld()->SpawnActor<AYDEquipment>(EquipmentClass, GetActorTransform(), SpawnParams);
		if (SpawnItem)
		{
			SpawnItem->CreateItem();
			Destroy();
		}	
	}
	
}

void AYDPickupItem::SetCommentWidget(bool bIsActive)
{
	if (CommentWidget)
	{
		CommentWidget->SetVisibility(bIsActive);
	}
}

void AYDPickupItem::ActiveComment(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	SetCommentWidget(true);
}

void AYDPickupItem::DeactivateComment(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SetCommentWidget(false);
}
