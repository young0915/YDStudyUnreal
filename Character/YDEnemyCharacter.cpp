// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/YDEnemyCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "AI/YDEnemyAIController.h"
#include "Components/WidgetComponent.h"
#include "Components/YDAttributeComponent.h"
#include "Components/YDCombatComponent.h"
#include "Components/YDStateComponent.h"
#include "Cores/YDGameplayTags.h"
#include "Datas/YDGameSingleton.h"
#include "Engine/DamageEvents.h"
#include "Perception/AISense_Damage.h"
#include "UI/YDStatBarWidget.h"


AYDEnemyCharacter::AYDEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	TargetSphereComponent = CreateDefaultSubobject<USphereComponent>(FName("TargetingSphereComponent"));
	TargetSphereComponent->SetupAttachment(GetRootComponent());
	TargetSphereComponent->SetCollisionObjectType(COLLISION_OBJECT_TARGETING);
	TargetSphereComponent->SetCollisionResponseToChannels(ECR_Ignore);
	TargetSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("LockOnWidget"));
	LockOnWidgetComponent->SetupAttachment(GetRootComponent());
	LockOnWidgetComponent->SetRelativeLocation(FVector(0, 0, 50));
	LockOnWidgetComponent->SetDrawSize(FVector2D(30, 30));
	LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidgetComponent->SetVisibility(false);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("HealthBarWidget"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 100));
	HealthBarWidgetComponent->SetDrawSize(FVector2D(100, 5.f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetVisibility(false);

	AttributeComponent = CreateDefaultSubobject<UYDAttributeComponent>(TEXT("Attribute"));
	AttributeComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
	AttributeComponent->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChanged);

	StateComponent = CreateDefaultSubobject<UYDStateComponent>(TEXT("State"));
	RotationComponent = CreateDefaultSubobject<UYDRotationComponent>(TEXT("Rotation"));
}

void AYDEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon();
	SetupHealthBar();
	ApplyStat();
}

void AYDEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AYDEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                    class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	
	if (AttributeComponent)
	{
		AttributeComponent->TakeDamageAmount(ActualDamage);
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		FVector ShotDirection = PointDamageEvent->ShotDirection;
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;
		FVector HitLocation = PointDamageEvent->HitInfo.Location;

		ImpactEffect(ImpactPoint);

		UAISense_Damage::ReportDamageEvent(GetWorld(), this, EventInstigator->GetPawn(), ActualDamage, HitLocation,
		                                   HitLocation);

		HitReaction(EventInstigator->GetPawn());
	}

	return ActualDamage;
}

void AYDEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(ParriedDelayTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StunnedDelayTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void AYDEnemyCharacter::OnDeath()
{
	if (AYDEnemyAIController* AIController = Cast<AYDEnemyAIController>(GetController()))
	{
		AIController->GetBrainComponent()->StopLogic(TEXT("Death"));
	}

	Super::OnDeath();

	if (AYDWeapon* MainWeapon = CombatComponent->GetMainWeapon())
	{
		MainWeapon->Drop();
	}

	SetDeathState();
}

void AYDEnemyCharacter::HitReaction(const AActor* Attacker)
{
	check(StateComponent);

	float StunnedDelay = 0.1f;
	if (StunnedRate >= FMath::RandRange(1, 100))
	{
		StateComponent->SetState(YDGameplayTags::Character_State_Stunned);
		StunnedDelay = FMath::FRandRange(0.5f, 3.f);
	}

	if (UAnimMontage* HitReactAnimMontage = CombatComponent->GetMainWeapon()->GetHitReactMontage(Attacker))
	{
		const float DelaySeconds = PlayAnimMontage(HitReactAnimMontage) + StunnedDelay;

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]()
		{
			FGameplayTagContainer CheckTags;
			CheckTags.AddTag(YDGameplayTags::Character_State_Stunned);
			if (StateComponent->IsCurrentStateEqualsToAny(CheckTags))
			{
				StateComponent->ClearState();
			}
		});
		GetWorld()->GetTimerManager().SetTimer(StunnedDelayTimerHandle, TimerDelegate, DelaySeconds, false);
	}
}

void AYDEnemyCharacter::SetDeathState()
{
	if (StateComponent)
	{
		StateComponent->SetState(YDGameplayTags::Character_State_Death);
	}

	if (AYDEnemyAIController* AIController = Cast<AYDEnemyAIController>(GetController()))
	{
		AIController->StopUpdateTarget();
	}

	ToggleHealthBarVisibility(false);
}

void AYDEnemyCharacter::OnAttributeChanged(EYDAttributeType AttributeType, float InValue)
{
	if (AttributeType == EYDAttributeType::Health)
	{
		if (HealthBarWidgetComponent)
		{
			if (const UYDStatBarWidget* StatBar = Cast<UYDStatBarWidget>(HealthBarWidgetComponent->GetWidget()))
			{
				StatBar->SetRatio(InValue);
			}
		}

		if (DefaultWeaponClasses.Num() > 0)
		{
			EYDCombatType CurrentCombatType = CombatComponent->GetMainWeapon()->GetCombatType();
			EYDCombatType NextCombatType = EYDCombatType::Unarmed;
			bool isChanging = false;
		
			for (const TPair<float, EYDCombatType> KV : PhaseRule)
			{
				if (InValue < KV.Key)
				{
					NextCombatType = KV.Value;
					isChanging =true;
				}
			}
		
			if (isChanging && NextCombatType != CurrentCombatType)
			{
				CombatComponent->SwapWeapon(NextCombatType);
			}
		}
	}
}

void AYDEnemyCharacter::SetupHealthBar()
{
	if (AttributeComponent)
	{
		AttributeComponent->BroadcastAttributeChanged(EYDAttributeType::Health);
	}
}


void AYDEnemyCharacter::OnTargeted(bool bTargeted)
{
	if (LockOnWidgetComponent)
	{
		LockOnWidgetComponent->SetVisibility(bTargeted);
	}
}

bool AYDEnemyCharacter::CanBeTargeted()
{
	if (!StateComponent)
	{
		return false;
	}

	FGameplayTagContainer TagCheck;
	TagCheck.AddTag(YDGameplayTags::Character_State_Death);
	return StateComponent->IsCurrentStateEqualsToAny(TagCheck) == false;
}

void AYDEnemyCharacter::ActivateWeaponCollision(EWeaponCollisionType InWeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->ActivateCollision(InWeaponCollisionType);
	}
}

void AYDEnemyCharacter::DeactivateWeaponCollision(EWeaponCollisionType InWeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->DeactivateCollision(InWeaponCollisionType);
	}
}

void AYDEnemyCharacter::PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate)
{
	check(StateComponent);
	check(CombatComponent);
	check(AttributeComponent);
	//
	// FGameplayTagContainer CheckTags;
	// CheckTags.AddTag(YDGameplayTags::Character_State_Stunned);
	// if (StateComponent->IsCurrentStateEqualsToAny(CheckTags))
	// {
	// 	return;
	// }
	//
	// if (const AYDWeapon* Weapon = CombatComponent->GetMainWeapon())
	// {
	// 	StateComponent->SetState(YDGameplayTags::Character_State_Attacking);
	// 	CombatComponent->SetLastAttackType(AttackTypeTag);
	// 	AttributeComponent->ToggleStaminaRegeneration(false);
	//
	// 	if (UAnimMontage* Montage = Weapon->GetRandomMontageForTag(AttackTypeTag))
	// 	{
	// 		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	// 		{
	// 			AnimInstance->Montage_Play(Montage);
	// 			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Montage);
	// 		}
	// 	}
	//
	// 	const float StaminaCost = Weapon->GetStaminaCost(AttackTypeTag);
	// 	AttributeComponent->DecreaseStamina(StaminaCost);
	// 	AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	// }

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(YDGameplayTags::Character_State_Stunned);
	if (StateComponent->IsCurrentStateEqualsToAny(CheckTags))
	{
		return;
	}

	if (const AYDWeapon* Weapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->SetState(YDGameplayTags::Character_State_Attacking);
		CombatComponent->SetLastAttackType(AttackTypeTag);
		AttributeComponent->ToggleStaminaRegeneration(false);

		if (UAnimMontage* Montage = Weapon->GetRandomMontageForTag(AttackTypeTag))
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(Montage);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Montage);
			}
		}

		const float StaminaCost = Weapon->GetStaminaCost(AttackTypeTag);
		AttributeComponent->DecreaseStamina(StaminaCost);
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void AYDEnemyCharacter::Parried()
{
	check(StateComponent);
	check(CombatComponent);

	StopAnimMontage();
	StateComponent->SetState(YDGameplayTags::Character_State_Parried);
	UE_LOG(LogTemp, Warning, TEXT("YDGameplayTags::Character_StateChanged 1 "));

	if (const AYDWeapon* MainWeapon = CombatComponent->GetMainWeapon())
	{
		UAnimMontage* ParriedAnimMontage = MainWeapon->GetMontageForTag(YDGameplayTags::Character_Action_ParriedHit);
		const float Delay = PlayAnimMontage(ParriedAnimMontage) + 1.0f;

		UE_LOG(LogTemp, Warning, TEXT("YDGameplayTags::Character_StateChanged 2"));

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]()
		{
			FGameplayTagContainer CheckTags;
			CheckTags.AddTag(YDGameplayTags::Character_State_Death);
			if (!StateComponent->IsCurrentStateEqualsToAny(CheckTags))
			{
				StateComponent->ClearState();
			}
		});
		GetWorld()->GetTimerManager().SetTimer(ParriedDelayTimerHandle, TimerDelegate, Delay, false);
	}
}

void AYDEnemyCharacter::ToggleHealthBarVisibility(bool bVisibility)
{
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(bVisibility);
	}
}

void AYDEnemyCharacter::SeesTarget(AActor* InTargetActor)
{
}

void AYDEnemyCharacter::ApplyStat()
{
	Super::ApplyStat();
	float StunRate = UYDGameSingleton::Get().GetCharacterStatData(CharacterID).StunRate;
}

void AYDEnemyCharacter::EquipWeapon()
{
	for (int32 i = 0; i < DefaultWeaponClasses.Num(); i++)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		AYDWeapon* Weapon = GetWorld()->SpawnActor<AYDWeapon>(DefaultWeaponClasses[i], GetActorTransform(), Params);
		Weapon->CreateItem();
	}

	CombatComponent->SwapWeapon(CombatComponent->GetMainWeapon()->GetCombatType());
}
