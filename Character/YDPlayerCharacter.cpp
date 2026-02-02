// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/YDPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ISkeletonTree.h"
#include "Animation/YDAnimInstance.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Cores/YDGameplayTags.h"
#include "Datas/YDGameSingleton.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/YDInteract.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Equipments/YDWeapon.h"
#include "Items/YDPickupItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/YDInventoryWidget.h"

AYDPlayerCharacter::AYDPlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.f, 0.f);
	
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	AttributeComponent = CreateDefaultSubobject<UYDAttributeComponent>(TEXT("Attribute"));
	AttributeComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);

	StateComponent = CreateDefaultSubobject<UYDStateComponent>(TEXT("State"));
	TargetingComponent = CreateDefaultSubobject<UYDTargetingComponent>(TEXT("Targeting"));
	InventoryComponent = CreateDefaultSubobject<UYDInventoryComponent>(TEXT("Inventory"));

	CombatComponent->OnPlayEquipMontage.AddUObject(this, &ThisClass::OnEquipAnimation);
}

void AYDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UYDPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UYDInventoryWidget>(GetWorld(), InventoryWidgetClass);
	}

	ApplyStat();
}


void AYDPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AYDPlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AYDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::Sprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ThisClass::SwapWeapon);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ThisClass::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::SpecialAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ThisClass::HeavyAttack);

		EnhancedInputComponent->BindAction(LockOnTargetAction, ETriggerEvent::Started, this, &ThisClass::LockOnTarget);

		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ThisClass::Blocking);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ThisClass::BlockingEnd);

		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &ThisClass::Parrying);

		EnhancedInputComponent->BindAction(ConsumeAction, ETriggerEvent::Started, this, &ThisClass::Consume);

		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ThisClass::ToggleInventory);
		EnhancedInputComponent->BindAction(NextSubSlotItemAction, ETriggerEvent::Started, this, &ThisClass::NextSubItemSlot);
	}
}

bool AYDPlayerCharacter::IsDeath() const
{
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(YDGameplayTags::Character_State_Death);
	return StateComponent->IsCurrentStateEqualsToAny(CheckTags);
}

float AYDPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                     class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	check(AttributeComponent);
	check(StateComponent);
	
	 bFacingEnemy =UKismetMathLibrary::InRange_FloatFloat( GetDotProductTo(EventInstigator->GetPawn()), -0.1f, 1);
	
	// Parrying
	if (ParriedAttackSucceed())
	{
		if (IYDCombatInterface* CombatInterface = Cast<IYDCombatInterface>(EventInstigator->GetPawn()))
		{
			CombatInterface->Parried();

			AYDWeapon* MainWepaon = CombatComponent->GetMainWeapon();
			if (IsValid(MainWepaon))
			{
				FVector Location = MainWepaon->GetActorLocation();
				ShieldBlockingEffect(Location);
			}
		}
		return ActualDamage;
	}

	if (CanPerformAttackBlocking())
	{
		AttributeComponent->TakeDamageAmount(0.0f);

		AttributeComponent->DecreaseStamina(20.0f);
		StateComponent->SetState(YDGameplayTags::Character_State_Blocking);
	}
	else
	{
		AttributeComponent->TakeDamageAmount(ActualDamage);
		StateComponent->SetState(YDGameplayTags::Character_State_Hit);
	}

	StateComponent->ToggleMovementInput(false);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		FVector ShotDirection = PointDamageEvent->ShotDirection;
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;
		FVector HitLocation = PointDamageEvent->HitInfo.Location;

		ImpactEffect(ImpactPoint);
		HitReaction(EventInstigator->GetPawn(), EYDDamageType::HitBack);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		const FRadialDamageEvent* RadialDamageEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);

		const FVector HitLocation = RadialDamageEvent->Origin;

		ImpactEffect(HitLocation);
		HitReaction(EventInstigator->GetPawn(), EYDDamageType::Knockback);
	}

	return ActualDamage;
}

void AYDPlayerCharacter::ShieldBlockingEffect(const FVector& Location)
{
}

void AYDPlayerCharacter::HitReaction(const AActor* Attacker, const EYDDamageType InDamageType)
{
	check(CombatComponent);

	if (CanPerformAttackBlocking())
	{
		if (UAnimMontage* HitReactAnimMontage = CombatComponent->GetMainWeapon()->GetMontageForTag(
			YDGameplayTags::Character_Action_BlockingHit))
		{
			PlayAnimMontage(HitReactAnimMontage);
		}
	}
	else
	{
		if (InDamageType == EYDDamageType::HitBack)
		{
			if (UAnimMontage* HitReactAnimMontage = CombatComponent->GetMainWeapon()->GetHitReactMontage(Attacker))
			{
				PlayAnimMontage(HitReactAnimMontage);
			}
		}
		else if (InDamageType == EYDDamageType::Knockback)
		{
			if (UAnimMontage* HitReactAnimMontage = CombatComponent->GetMainWeapon()->GetMontageForTag(
				YDGameplayTags::Character_Action_KnockBackHit))
			{
				PlayAnimMontage(HitReactAnimMontage);
			}
		}
	}
}

void AYDPlayerCharacter::OnDeath()
{
	Super::OnDeath();
}


void AYDPlayerCharacter::Move(const FInputActionValue& Values)
{
	check(StateComponent);
	if (StateComponent->MovementInputEnabled() == false)
	{
		return;
	}

	FVector2D MovementVector = Values.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotator(0, Rotation.Yaw, 0);

		const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void AYDPlayerCharacter::Look(const FInputActionValue& Values)
{
	if (TargetingComponent && TargetingComponent->IsLockOn())
	{
		return;
	}

	FVector2D LookDirection = Values.Get<FVector2D>();
	float PitchInput = LookDirection.Y;

	float ControlPitch = 0.f;
	if (Controller != nullptr)
	{
		const FRotator ContorlRot = Controller->GetControlRotation();
		ControlPitch = FRotator::NormalizeAxis(ContorlRot.Pitch);
	}

	if (ControlPitch >= 45.0f && ControlPitch <= 90.0f)
	{
		if (PitchInput < 0.0f)
		{
			PitchInput = 0;
		}
	}

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(PitchInput);
	}
}

void AYDPlayerCharacter::Sprinting()
{
	check(AttributeComponent);
	check(CombatComponent);

	if (CombatComponent->IsBlockingEnabled())
	{
		return;
	}

	if (AttributeComponent->CheckHasEnoughStamina(5.f) && IsMoving())
	{
		AttributeComponent->ToggleStaminaRegeneration(false);
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
		AttributeComponent->DecreaseStamina(0.1f);
		bSprinting = true;
	}
	else
	{
		StopSprint();
	}
}


void AYDPlayerCharacter::StopSprint()
{
	check(AttributeComponent);
	check(CombatComponent);

	if (CombatComponent->IsBlockingEnabled())
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	AttributeComponent->ToggleStaminaRegeneration(true);
	bSprinting = false;
}

void AYDPlayerCharacter::Interact()
{
	FHitResult OutHit;
	const FVector Start = GetActorLocation();
	const FVector End = Start;
	constexpr float Radius = 100.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACTION));
	TArray<AActor*> ActorsToIgnore;

	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this, Start, End, Radius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

	if (bHit)
	{
		if (AActor* HitActor = OutHit.GetActor())
		{
			if (IYDInteract* Interaction = Cast<IYDInteract>(HitActor))
			{
				Interaction->Interact(this);
			}
		}
	}
}

void AYDPlayerCharacter::SwapWeapon()
{
	check(CombatComponent);
	check(StateComponent);

	if (CombatComponent)
	{
		if (CanSwapWeapon())
		{
			StateComponent->SetState(YDGameplayTags::Character_State_GeneralAction);
			StateComponent->ToggleMovementInput(false);
			CombatComponent->SwapWeapon();
		}
	}
}

void AYDPlayerCharacter::Attack()
{
	const FGameplayTag AttackTypeTag = GetAttackPerform();

	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void AYDPlayerCharacter::SpecialAttack()
{
	const FGameplayTag AttackTypeTag = YDGameplayTags::Character_Attack_Special;
	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void AYDPlayerCharacter::HeavyAttack()
{
	const FGameplayTag AttackTypeTag = YDGameplayTags::Character_Attack_Heavy;
	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void AYDPlayerCharacter::LockOnTarget()
{
	TargetingComponent->ToggleLockOn();
}

void AYDPlayerCharacter::Blocking()
{
	check(CombatComponent);
	check(StateComponent);

	if (CombatComponent->GetMainWeapon())
	{
		if (CanPlayerBlockStance())
		{
			GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
			CombatComponent->SetBlockingEnabled(true);

			if (UYDAnimInstance* AnimInstance = Cast<UYDAnimInstance>(GetMesh()->GetAnimInstance()))
			{
				AnimInstance->UpdateBlocking(true);
				StateComponent->SetState(YDGameplayTags::Character_State_Blocking);
			}
		}
	}
}

void AYDPlayerCharacter::BlockingEnd()
{
	check(CombatComponent);
	check(StateComponent);

	CombatComponent->SetBlockingEnabled(false);
	if (UYDAnimInstance* AnimInstance = Cast<UYDAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->UpdateBlocking(false);
		StateComponent->ClearState();
	}
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void AYDPlayerCharacter::Parrying()
{
	check(CombatComponent);
	check(StateComponent);
	check(AttributeComponent);

	if (CanPerformParry())
	{
		if (const AYDWeapon* MainWeapon = CombatComponent->GetMainWeapon())
		{
			UAnimMontage* ParryingMontage = MainWeapon->GetMontageForTag(YDGameplayTags::Character_State_Parrying);

			StateComponent->ToggleMovementInput(false);
			AttributeComponent->ToggleStaminaRegeneration(false);
			AttributeComponent->DecreaseStamina(10.0f);

			PlayAnimMontage(ParryingMontage);
		
			AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
		}
	}
}

void AYDPlayerCharacter::Consume()
{
	if (!StateComponent)
	{
		return;
	}

	if (CanUseItem())
	{
		if (UYDItemObject* Item = InventoryComponent->GetSubBagItem())
		{
			if (Item->Data.ItemType == EYDItemType::Potion)
			{
				StateComponent->SetState(YDGameplayTags::Character_State_UsePotion);
				if (UseItemMontage != nullptr)
				{
					PlayAnimMontage(UseItemMontage);
				}
			}
			else if (Item->Data.ItemType == EYDItemType::Projectile)
			{
				StateComponent->SetState(YDGameplayTags::Character_State_TrowItem);
				if (ThrowItemMontage != nullptr)
				{
					PlayAnimMontage(ThrowItemMontage);
				}
			}

			InventoryComponent->UseSubBagItem();
		}
		
	
	}
}

void AYDPlayerCharacter::ToggleInventory()
{
	check(InventoryComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !InventoryWidget)
	{
		return;
	}

	const bool bVisible = (InventoryWidget->GetVisibility() == ESlateVisibility::Visible);

	if (bVisible)
	{
		// Close
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		InventoryWidget->RemoveFromParent();
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		// Open
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryWidget->AddToViewport();
		PC->bShowMouseCursor = true;

		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		Mode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(Mode);
	}
}

void AYDPlayerCharacter::NextSubItemSlot()
{
	if (PlayerHUDWidget !=nullptr)
	{
		PlayerHUDWidget->AdvanceSubItemIndex();
	}
}


FGameplayTag AYDPlayerCharacter::GetAttackPerform() const
{
	if (IsSprinting())
	{
		return YDGameplayTags::Character_Attack_Running;
	}

	return YDGameplayTags::Character_Attack_Light;
}

bool AYDPlayerCharacter::CanPerformAttack(const FGameplayTag& AttackTypeTag) const
{
	check(StateComponent);
	check(CombatComponent);
	check(AttributeComponent);

	if (IsValid(CombatComponent->GetMainWeapon()) == false)
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(YDGameplayTags::Character_State_GeneralAction);
	CheckTags.AddTag(YDGameplayTags::Character_State_Hit);
	CheckTags.AddTag(YDGameplayTags::Character_State_Blocking);

	const float StaminaCost = CombatComponent->GetMainWeapon()->GetStaminaCost(AttackTypeTag);

	return StateComponent->IsCurrentStateEqualsToAny(CheckTags) == false
		&& AttributeComponent->CheckHasEnoughStamina(StaminaCost);
}

void AYDPlayerCharacter::DoAttack(const FGameplayTag& AttackTypeTag)
{
	check(StateComponent);
	check(CombatComponent);
	check(AttributeComponent);

	if (const AYDWeapon* Weapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->SetState(YDGameplayTags::Character_State_Attacking);
		StateComponent->ToggleMovementInput(false);
		CombatComponent->SetLastAttackType(AttackTypeTag);

		AttributeComponent->ToggleStaminaRegeneration(false);

		UAnimMontage* Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		if (!Montage)
		{
			ComboCounter = 0;
			Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		}

		PlayAnimMontage(Montage);

		const float StaminaCost = Weapon->GetStaminaCost(AttackTypeTag);
		AttributeComponent->DecreaseStamina(StaminaCost);
		AttributeComponent->ToggleStaminaRegeneration(true, 1.0f);
	}
}

void AYDPlayerCharacter::ExecuteComboAttack(const FGameplayTag& AttackTypeTag)
{
	if (StateComponent->GetCurrentState() != YDGameplayTags::Character_State_Attacking)
	{
		if (bComboSequenceRunning && bCanComboInput == false)
		{
			ComboCounter++;
		}
		else
		{
			ResetCombo();
			bComboSequenceRunning = true;
		}
		DoAttack(AttackTypeTag);
		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	}
	else if (bCanComboInput)
	{
		bSavedComboInput = true;
	}
}

void AYDPlayerCharacter::ResetCombo()
{
	bComboSequenceRunning = false;
	bCanComboInput = false;
	bSavedComboInput = false;
	ComboCounter = 0;
}

bool AYDPlayerCharacter::CanPlayerBlockStance() const
{
	check(CombatComponent);
	check(StateComponent);
	check(AttributeComponent);

	if (IsSprinting())
	{
		return false;
	}

	AYDWeapon* Weapon = CombatComponent->GetMainWeapon();
	if (!IsValid(Weapon))
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(YDGameplayTags::Character_State_Attacking);
	CheckTags.AddTag(YDGameplayTags::Character_State_GeneralAction);
	CheckTags.AddTag(YDGameplayTags::Character_State_Hit);

	return StateComponent->IsCurrentStateEqualsToAny(CheckTags) == false &&
		AttributeComponent->CheckHasEnoughStamina(1.f);
}

bool AYDPlayerCharacter::CanPerformAttackBlocking() const
{
	check(CombatComponent);
	check(AttributeComponent);
	return bFacingEnemy && CombatComponent->IsBlockingEnabled() && AttributeComponent->CheckHasEnoughStamina(1.f);
}

bool AYDPlayerCharacter::CanPerformParry() const
{
	check(CombatComponent);
	check(StateComponent);
	check(AttributeComponent);

	AYDWeapon* Weapon = CombatComponent->GetMainWeapon();
	if (!IsValid(Weapon))
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(YDGameplayTags::Character_State_Attacking);
	CheckTags.AddTag(YDGameplayTags::Character_State_GeneralAction);
	CheckTags.AddTag(YDGameplayTags::Character_State_Hit);
	CheckTags.AddTag(YDGameplayTags::Character_State_Blocking);
	CheckTags.AddTag(YDGameplayTags::Character_State_Death);
	CheckTags.AddTag(YDGameplayTags::Character_State_Parrying);

	return StateComponent->IsCurrentStateEqualsToAny(CheckTags) == false &&
		AttributeComponent->CheckHasEnoughStamina(1.f);
}

bool AYDPlayerCharacter::ParriedAttackSucceed() const
{
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(YDGameplayTags::Character_State_Parrying);

	// const bool bFront = UKismetMathLibrary::InRange_FloatFloat(GetDotProductTo(EventInstigator->GetPawn()),  0.2f, 1.f);
	// const bool bBack  = UKismetMathLibrary::InRange_FloatFloat(GetDotProductTo(EventInstigator->GetPawn()), -1.f, -0.8f);
	//

	//const bool 

	return StateComponent->IsCurrentStateEqualsToAny(CheckTags) && bFacingEnemy;
}

bool AYDPlayerCharacter::CanUseItem() const
{
	check(InventoryComponent);
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(YDGameplayTags::Character_State_Attacking);
	CheckTags.AddTag(YDGameplayTags::Character_State_Blocking);
	CheckTags.AddTag(YDGameplayTags::Character_State_Death);
	CheckTags.AddTag(YDGameplayTags::Character_State_GeneralAction);
	CheckTags.AddTag(YDGameplayTags::Character_State_Hit);
	CheckTags.AddTag(YDGameplayTags::Character_State_Parrying);

	return !(StateComponent->IsCurrentStateEqualsToAny(CheckTags));
}

void AYDPlayerCharacter::InterruptWhileUseItem() const
{
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(YDGameplayTags::Character_State_Attacking);
}

void AYDPlayerCharacter::ApplyStat()
{
	Super::ApplyStat();
	float Speed = UYDGameSingleton::Get().GetCharacterStatData(CharacterID).Speed;
	NormalSpeed = Speed;

}


void AYDPlayerCharacter::EnableCombWindow()
{
	bCanComboInput = true;
}

void AYDPlayerCharacter::DisableCombWindow()
{
	check(CombatComponent);
	bCanComboInput = false;

	if (bSavedComboInput)
	{
		bSavedComboInput = false;
		ComboCounter++;
		DoAttack(CombatComponent->GetLastAttackType());
	}
}

void AYDPlayerCharacter::AttackFinished(const float ComboResetDelay)
{
	if (StateComponent)
	{
		StateComponent->ToggleMovementInput(true);
	}

	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ThisClass::ResetCombo, ComboResetDelay, false);
}



void AYDPlayerCharacter::ActivateWeaponCollision(EWeaponCollisionType InWeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->ActivateCollision(InWeaponCollisionType);
	}
}

void AYDPlayerCharacter::DeactivateWeaponCollision(EWeaponCollisionType InWeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->DeactivateCollision(InWeaponCollisionType);
	}
}

void AYDPlayerCharacter::OnEquipAnimation()
{
	if (CombatComponent->GetMainWeapon() == nullptr)
	{
		return;
	}

	if (AYDWeapon* Weapon = CombatComponent->GetMainWeapon())
	{
		PlayAnimMontage(Weapon->GetMontageForTag(YDGameplayTags::Character_Action_Equip));
	}
}

bool AYDPlayerCharacter::IsMoving() const
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		return MovementComp->Velocity.Size2D() > 3.f && MovementComp->GetCurrentAcceleration() != FVector::Zero();
	}

	return false;
}

bool AYDPlayerCharacter::CanSwapWeapon() const
{
	check(StateComponent);
	if (!IsValid(CombatComponent->GetMainWeapon()))
	{
		return false;
	}

	if (CombatComponent->GetMainWeapon()->GetCombatType() == EYDCombatType::Unarmed)
	{
		return false;
	}

	if (!CombatComponent->HasMultipleWeapons())
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(YDGameplayTags::Character_State_Attacking);
	CheckTags.AddTag(YDGameplayTags::Character_State_GeneralAction);

	return (StateComponent->IsCurrentStateEqualsToAny(CheckTags) == false);
}
