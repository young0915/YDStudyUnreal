// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/YDItemProjectile.h"

#include "Datas/YDGameSingleton.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


AYDItemProjectile::AYDItemProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetCollisionProfileName(TEXT("Projectile"));
	Mesh->OnComponentHit.AddDynamic(this, &AYDItemProjectile::OnMeshHit);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->Deactivate();
	MovementComponent->InitialSpeed = 0;

	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Damage Sphere"));
	DamageSphere->SetupAttachment(RootComponent);
	DamageSphere->InitSphereRadius(10.f);
	DamageSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageSphere->SetCollisionObjectType(ECC_WorldDynamic);
	DamageSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	

}

void AYDItemProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* OwnerActor = GetOwner())
	{
		Mesh->IgnoreActorWhenMoving(OwnerActor, true);
	}

	if (AActor* Inst = GetInstigator())
	{
		Mesh->IgnoreActorWhenMoving(Inst, true);
	}

	ExplosionDamage = UYDGameSingleton::Get().GetItemData(ItemCode).Attack;
}

void AYDItemProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (UPrimitiveComponent* Updated = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		MovementComponent->SetUpdatedComponent(Updated);
	}
}


void AYDItemProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AYDItemProjectile::ThrowProjectile(const FVector& Dir, float Speed)
{
	if (!MovementComponent)
	{
		return;
	}

	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		Prim->SetSimulatePhysics(false);
		Prim->SetEnableGravity(false);
		Prim->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Prim->SetCollisionProfileName(TEXT("Projectile"));
	}

	MovementComponent->InitialSpeed = Speed;
	MovementComponent->Velocity = Dir.GetSafeNormal() * Speed;
	MovementComponent->Activate(true);
}

void AYDItemProjectile::EndDamageWindow()
{
	DamageSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AYDItemProjectile::OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == this || OtherActor == GetInstigator() || OtherActor == GetOwner())
	{
		return;
	}

	Explode(Hit);
}


void AYDItemProjectile::Explode(const FHitResult& Hit)
{
	if (MovementComponent)
	{
		MovementComponent->StopMovementImmediately();
		Mesh->SetVisibility(false, true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (ExplosionFx)
	{
		UGameplayStatics::SpawnEmitterAttached(
			ExplosionFx,
			GetRootComponent(),
			NAME_None,
			Hit.ImpactPoint,
			FRotator::ZeroRotator,
			FVector(1.0f),
			EAttachLocation::KeepWorldPosition,
			true);
	}

	if (DamageSphere)
	{
		DamageSphere->SetWorldLocation(Hit.ImpactPoint);
		DamageSphere->SetSphereRadius(ExplosionRadius);
		DamageSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		DamageSphere->SetGenerateOverlapEvents(true);
		DamageSphere->UpdateOverlaps();

		TArray<AActor*> Overlaps;
		DamageSphere->GetOverlappingActors(Overlaps, ACharacter::StaticClass());
		AActor* Ignored = GetInstigator();
		for (AActor* A : Overlaps)
		{
			if (!A || A == this || A == Ignored)
			{
				continue;
			}

			UGameplayStatics::ApplyDamage(A, ExplosionDamage,
			                              GetInstigatorController(), this, DamageTypeClass);
		}
	}

	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AYDItemProjectile::EndDamageWindow, 0.5f, false);


	SetLifeSpan(0.5f);
}
