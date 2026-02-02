// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/YDWeapon.h"

#include "Animation/YDAnimInstance.h"
#include "Character/YDPlayerCharacter.h"
#include "Datas/YDMontageActionData.h"
#include "Components/YDCombatComponent.h"
#include "Cores/YDGameplayTags.h"
#include "Datas/YDGameSingleton.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AYDWeapon::AYDWeapon()
{
	WeaponCollision = CreateDefaultSubobject<UYDWeaponCollisionComponent>(TEXT("WeaponCollision"));
	WeaponCollision->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);

	SecondWeaponCollision = CreateDefaultSubobject<UYDWeaponCollisionComponent>(TEXT("SecondWeaponCollision"));
	SecondWeaponCollision->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);

}

void AYDWeapon::BeginPlay()
{
	Super::BeginPlay();
	ApplyStat();
}

void AYDWeapon::EquipItem()
{
	Super::EquipItem();
	CombatComponent = GetOwner()->GetComponentByClass<UYDCombatComponent>();
	if (CombatComponent)
	{
		if (AYDCharacterBase* Character = Cast<AYDCharacterBase>(GetOwner()))
		{
			if (UYDAnimInstance* Anim = Cast<UYDAnimInstance>(Character->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}
		}

		CombatComponent->SetWeapon(this);
		SetActiveEquipment(true);

		if (CombatType == EYDCombatType::SwordShield || CombatType == EYDCombatType::Axe)
		{
			EnsureOffHandEquipped();
		}

		ApplyStat();
	}
}

void AYDWeapon::CreateItem()
{
	CombatComponent = GetOwner()->GetComponentByClass<UYDCombatComponent>();
	InventoryComponent = GetOwner()->GetComponentByClass<UYDInventoryComponent>();

	if (CombatComponent)
	{
		AttachToOwner(EquipSocketName);

		WeaponCollision->SetWeaponMesh(Mesh);
		WeaponCollision->AddIgnoreActor(GetOwner());
		SetActiveEquipment(false);
		
		if (::IsValid(CombatComponent->GetMainWeapon()) &&
			CombatComponent->GetMainWeapon()->GetCombatType() == EYDCombatType::Unarmed)
		{
			EquipItem();
		}
		else
		{
			CombatComponent->AddWeapon(CombatType, this);
		}

		if (InventoryComponent )
		{
			UYDItemObject* NewItem = NewObject<UYDItemObject>();
			NewItem->Data.ItemCode = ItemCode;

			InventoryComponent->AddItem(this);
			
		}
	}
}

void AYDWeapon::Drop()
{
	if (CombatType != EYDCombatType::Unarmed)
	{
		DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetSimulatePhysics(true);
	}
}

void AYDWeapon::EnsureOffHandEquipped()
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		LeftWeaponMesh = NewObject<UStaticMeshComponent>(Character->GetMesh());
		if (CombatType == EYDCombatType::Axe)
		{
			LeftWeaponMesh->SetStaticMesh(Mesh->GetStaticMesh());
			LeftWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			LeftWeaponMesh->SetGenerateOverlapEvents(false);
			LeftWeaponMesh->SetGenerateOverlapEvents(true);
			LeftWeaponMesh->RegisterComponent();

			LeftWeaponMesh->AttachToComponent(Character->GetMesh(),
			                                  FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                                  "Axe_Equip_Socket_l");
			LeftWeaponMesh->SetVisibility(true);
		}
		else if (CombatType == EYDCombatType::SwordShield)
		{
			if (AYDShield* MainShield =CombatComponent->GetShield())
			{
				MainShield->EquipItem();
			}
			else
			{
				// Create Shield
				if (SubEquipmentClass)
				{
					if (UClass* Cls = SubEquipmentClass.Get())
					{
						if (Cls->IsChildOf(AYDShield::StaticClass()))
						{
							TSubclassOf<AYDShield> ShieldClass = Cls;
							FActorSpawnParameters SpawnParams;
							SpawnParams.Owner = Character;
							SpawnParams.Instigator = Character->GetInstigator();
							SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
							AYDShield* Shield = GetWorld()->SpawnActor<AYDShield>(
								ShieldClass, Character->GetActorTransform(), SpawnParams);
							Shield->EquipItem();
						}
					}
				}
			}
			
		}
	}
}

void AYDWeapon::ApplyStat()
{
	if (AYDCharacterBase* Character = Cast<AYDCharacterBase>(GetOwner()))
	{
		int32 CharacterId = Character->GetCharacterID();

		if (CharacterId <0 || ItemCode <0)
		{
			return;
		}

		StaminaCostMap.Reset();
		DamageMultiplierMap.Reset();

	
		float LightAttack = UYDGameSingleton::Get().GetItemData(ItemCode).Attack;
		float HeavyAttack = UYDGameSingleton::Get().GetItemData(ItemCode).HeavyAttack;
		float RunningAttack = UYDGameSingleton::Get().GetItemData(ItemCode).Attack;
		float SpecialAttack = UYDGameSingleton::Get().GetItemData(ItemCode).SpecialAttack;

		float LightStamina = UYDGameSingleton::Get().GetItemData(ItemCode).StaminaDamage;
		float HeavyStamina = UYDGameSingleton::Get().GetItemData(ItemCode).HeavyStamina;
		float RunningStamina = UYDGameSingleton::Get().GetItemData(ItemCode).StaminaDamage;
		float SpecialStamina = UYDGameSingleton::Get().GetItemData(ItemCode).SpecialStamina;
		float TraceRadius =  UYDGameSingleton::Get().GetItemData(ItemCode).TraceRadius;

		StaminaCostMap.Add(YDGameplayTags::Character_Attack_Light, LightStamina);
		StaminaCostMap.Add(YDGameplayTags::Character_Attack_Heavy, HeavyStamina);
		StaminaCostMap.Add(YDGameplayTags::Character_Attack_Running, RunningStamina);
		StaminaCostMap.Add(YDGameplayTags::Character_Attack_Special, SpecialStamina);
	
		DamageMultiplierMap.Add(YDGameplayTags::Character_Attack_Heavy, HeavyAttack);
		DamageMultiplierMap.Add(YDGameplayTags::Character_Attack_Running, RunningAttack);
		DamageMultiplierMap.Add(YDGameplayTags::Character_Attack_Special, SpecialAttack);

		float Damage = UYDGameSingleton::Get().GetCharacterStatData(CharacterId).Attack + LightAttack;
		BaseDamage = Damage;

		Defense   = UYDGameSingleton::Get().GetItemData(ItemCode).Defense;

		if (WeaponCollision)
		{
			WeaponCollision->SetTraceRadius(TraceRadius);
		}

		if (SecondWeaponCollision)
		{
			SecondWeaponCollision->SetTraceRadius(TraceRadius);
		}

	}
}

UAnimMontage* AYDWeapon::GetMontageForTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageActionData->GetMontageForTag(Tag, Index);
}

UAnimMontage* AYDWeapon::GetRandomMontageForTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageActionData->GetRandomMontageForTag(Tag);
}

UAnimMontage* AYDWeapon::GetHitReactMontage(const AActor* Attacker) const
{
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		GetOwner()->GetActorLocation(), Attacker->GetActorLocation());
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(
		GetOwner()->GetActorRotation(), LookAtRotation);
	const float DeltaZ = DeltaRotation.Yaw;

	EYDHitDirection HitDirection = EYDHitDirection::Front;

	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.0f, 45.0f))
	{
		HitDirection = EYDHitDirection::Front;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.0f, 135.0f))
	{
		HitDirection = EYDHitDirection::Left;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 135.0f, 180.0f) ||
		UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.0f, -135.0f))
	{
		HitDirection = EYDHitDirection::Back;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.0f, -45.0f))
	{
		HitDirection = EYDHitDirection::Right;
	}


	UAnimMontage* SelectedMontage = nullptr;

	int MontageIndex = static_cast<int>(HitDirection);

	if (MontageIndex >= 0 && MontageIndex < static_cast<int>(EYDHitDirection::Total))
	{
		SelectedMontage = GetMontageForTag(YDGameplayTags::Character_Action_HitReaction, MontageIndex);
	}

	return SelectedMontage;
}

float AYDWeapon::GetStaminaCost(const FGameplayTag& InTag) const
{
	if (StaminaCostMap.Contains(InTag))
	{
		return StaminaCostMap[InTag];
	}
	return 0.0f;
}

float AYDWeapon::GetAttackDamage() const
{
	if (const AActor* OwnerActor = GetOwner())
	{
		const FGameplayTag LastAttackType = CombatComponent->GetLastAttackType();

		if (DamageMultiplierMap.Contains(LastAttackType))
		{
			const float Multiplier = DamageMultiplierMap[LastAttackType];
			return BaseDamage * Multiplier;
		}
	}

	return BaseDamage;
}

bool AYDWeapon::GetIsEquip() const
{
	return (ItemRef == nullptr ? false : ItemRef->IsEquipped);
}

void AYDWeapon::ActivateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollision->TurnOnCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollision->TurnOnCollision();
		break;
	}
}

void AYDWeapon::DeactivateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollision->TurnOffCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollision->TurnOffCollision();
		break;
	}
}

void AYDWeapon::OnHitActor(const FHitResult& Hit)
{
	AActor* TargetActor = Hit.GetActor();

	FVector DamageDirection = GetOwner()->GetActorForwardVector();

	float AttackDamage = GetAttackDamage();

	UGameplayStatics::ApplyPointDamage(
		TargetActor,
		AttackDamage,
		DamageDirection,
		Hit,
		GetOwner()->GetInstigatorController(),
		this,
		nullptr);
}
