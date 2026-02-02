// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/YDCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/YDCombatComponent.h"
#include "Datas/YDGameSingleton.h"
#include "Equipments/YDUnarmed.h"
#include "Equipments/YDWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AYDCharacterBase::AYDCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	UStaticMeshComponent* Hair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh(), FName(TEXT("Head")));
	Hair->SetRelativeRotation(FRotator(0, 0, -90));
	Attachments.Add("Hair", Hair);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	CombatComponent = CreateDefaultSubobject<UYDCombatComponent>(TEXT("Combat"));
}

void AYDCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (UnarmedClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		AYDUnarmed* Unarmed = GetWorld()->SpawnActor<AYDUnarmed>(UnarmedClass, GetActorTransform(), SpawnParams);
		Unarmed->EquipItem();
	}
}

void AYDCharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetCharacterObjectInit();
}

void AYDCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AYDCharacterBase::SetCharacterObjectInit()
{
	if (CharacterObjectID < 0)
	{
		return;
	}
	const FYDCharacterObjectData CharacteData = UYDGameSingleton::Get().GetCharacterObject(CharacterObjectID);
	FSoftObjectPath MeshPath = CharacteData.CharacterMeshPath;

	if (MeshPath.IsValid())
	{
		USkeletalMesh* BodyLoadedMesh = LoadObject<USkeletalMesh>(nullptr, *MeshPath.ToString());
		if (BodyLoadedMesh)
		{
			GetMesh()->SetSkeletalMesh(BodyLoadedMesh);
		}
	}

	if (UStaticMesh* LoaedMesh = Cast<UStaticMesh>(CharacteData.HairPath.TryLoad()))
	{
		Attachments["Hair"]->SetStaticMesh(LoaedMesh);
	}
}

void AYDCharacterBase::ImpactEffect(const FVector& Location)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
	}

	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Location);
	}
}

void AYDCharacterBase::HitReaction(const AActor* Attacker)
{
	check(StateComponent);

	if (UAnimMontage* HitReactAnimMontage = CombatComponent->GetMainWeapon()->GetHitReactMontage(Attacker))
	{
		PlayAnimMontage(HitReactAnimMontage);
	}
}

void AYDCharacterBase::OnDeath()
{
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetCollisionProfileName("Ragdoll");
		MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		MeshComp->SetSimulatePhysics(true);
		MeshComp->SetAllBodiesSimulatePhysics(true);
		MeshComp->WakeAllRigidBodies();
		MeshComp->bIgnoreRadialForce = true;
		MeshComp->bIgnoreRadialImpulse = true;
	}
}

void AYDCharacterBase::ApplyStat()
{
	check(AttributeComponent);
	check(CombatComponent);

	if (CharacterID < 0)
	{
		return;
	}

	float Health = UYDGameSingleton::Get().GetCharacterStatData(CharacterID).Health;
	float Stamina = UYDGameSingleton::Get().GetCharacterStatData(CharacterID).Stamina;
	float StaminaRegenRate = UYDGameSingleton::Get().GetCharacterStatData(CharacterID).StaminaRegenRate;
	float Defense = UYDGameSingleton::Get().GetCharacterStatData(CharacterID).Defense + CombatComponent->GetMainWeapon()
		->GetDefense();
	float MaxSpeed = UYDGameSingleton::Get().GetCharacterStatData(CharacterID).MaxSpeed;

	AttributeComponent->ApplyAttribute(Health, Stamina, StaminaRegenRate, Defense);
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}
