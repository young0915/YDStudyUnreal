// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/YDAttributeComponent.h"
#include "Components/YDStateComponent.h"
#include "Components/YDCombatComponent.h"
#include "Equipments/YDUnarmed.h"
#include "YDCharacterBase.generated.h"

UCLASS()
class YD_API AYDCharacterBase : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CharacterAttachment")
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Attachments;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UYDAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UYDStateComponent> StateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UYDCombatComponent> CombatComponent;

	//Effect Section 
protected:
	UPROPERTY(EditAnywhere, Category= "Effect")
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ImpactParticle;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AYDUnarmed> UnarmedClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Character Id")
	int32 CharacterID =-1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Character Id")
	// Character 외형 데이터 키  
	int32 CharacterObjectID = -1;
	
public:
	AYDCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetCharacterObjectInit();
	virtual void ImpactEffect(const FVector& Location);
	virtual void HitReaction(const AActor* Attacker);
	virtual void OnDeath();
	virtual void ApplyStat();
	

public :
	FORCEINLINE void SetMovementMode(EMovementMode NewMovementMode) { GetCharacterMovement()->SetMovementMode(NewMovementMode);}
	FORCEINLINE int32 GetCharacterID() const { return CharacterID; }

	
};

