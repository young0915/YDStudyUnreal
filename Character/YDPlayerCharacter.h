// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "YDCharacter.h"
#include "Camera/CameraComponent.h"
#include "Character/YDCharacterBase.h"
#include "Components/YDCombatComponent.h"
#include "Components/YDInventoryComponent.h"
#include "Components/YDStateComponent.h"
#include "Components/YDTargetingComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/YDCombatInterface.h"
#include "UI/YDPlayerHUDWidget.h"
#include "UI/YDInventoryWidget.h"
#include "YDPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class YD_API AYDPlayerCharacter : public AYDCharacterBase, public IYDCombatInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleCombatAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> HeavyAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LockOnTargetAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ParryAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ConsumeAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> NextSubSlotItemAction;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UYDTargetingComponent> TargetingComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UYDInventoryComponent> InventoryComponent;

	// UI Section
protected:
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UYDPlayerHUDWidget>	PlayerHUDWidget;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UYDInventoryWidget> InventoryWidget;

	// Sprint Section
protected:
	UPROPERTY(EditAnywhere, Category= "Sprinting")
	float SprintingSpeed = 750.0f;

	UPROPERTY(EditAnywhere, Category= "Sprinting")
	float NormalSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category= "Sprinting")
	bool bSprinting = false;

	// Combo Section
protected:
	bool bComboSequenceRunning = false;

	bool bCanComboInput = false;

	int32 ComboCounter = 0;

	bool bSavedComboInput = false;

	FTimerHandle ComboResetTimerHandle;

protected:
	bool bFacingEnemy = false;
	bool bBackEnemy = false;

protected:
	UPROPERTY(EditAnywhere, Category="Montage")
	UAnimMontage* UseItemMontage;

	UPROPERTY(EditAnywhere, Category="Montage")
	UAnimMontage* ThrowItemMontage;
public:
	AYDPlayerCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE UYDStateComponent* GetStateComponent() { return StateComponent; }
	FORCEINLINE UYDInventoryComponent* GetInventoryComponent() { return InventoryComponent; }
	FORCEINLINE UYDTargetingComponent* GetTargetingComponent() { return TargetingComponent; }
	FORCEINLINE void SetCharacterID(int32 InCharacterId)  { CharacterID = InCharacterId; }
	
	bool IsDeath() const;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

	void ShieldBlockingEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker, const EYDDamageType InDamageType);
	virtual void OnDeath() override;

protected:
	bool IsMoving() const;
	bool CanSwapWeapon() const;
	FORCEINLINE bool IsSprinting() const { return bSprinting; }

	void Move(const FInputActionValue& Values);
	void Look(const FInputActionValue& Values);
	void Sprinting();
	void StopSprint();
	void Interact();
	void SwapWeapon();

	void Attack();
	void SpecialAttack();
	void HeavyAttack();

	void LockOnTarget();

	void Blocking();
	void BlockingEnd();

	void Parrying();

	void Consume();

	void ToggleInventory();
	void NextSubItemSlot();

protected:
	FGameplayTag GetAttackPerform() const;

	bool CanPerformAttack(const FGameplayTag& AttackTypeTag) const;
	void DoAttack(const FGameplayTag& AttackTypeTag);
	void ExecuteComboAttack(const FGameplayTag& AttackTypeTag);

	void ResetCombo();

	bool CanPlayerBlockStance() const;
	bool CanPerformAttackBlocking() const;

	bool CanPerformParry() const;

	bool ParriedAttackSucceed() const;

	bool CanUseItem() const;

	void InterruptWhileUseItem() const;
	virtual void ApplyStat() override;

public :
	void EnableCombWindow();
	void DisableCombWindow();
	void AttackFinished(const float ComboResetDelay);
public:
	virtual void ActivateWeaponCollision(EWeaponCollisionType InWeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType InWeaponCollisionType) override;

private :
	void OnEquipAnimation();
};

