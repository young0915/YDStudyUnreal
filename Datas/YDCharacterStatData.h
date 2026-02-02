
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "YDCharacterStatData.generated.h"

USTRUCT(BlueprintType)
struct FYDCharacterStatData : public FTableRowBase
{
	GENERATED_BODY()

	FYDCharacterStatData() : Health(0.0f), Stamina(0.0f), StaminaRegenRate(0.0f), Attack(0.0f), AttackRange(0.0f),
							 Defense(0.0f), Speed(0.0f), MaxSpeed(0.0f), StunRate(0.0f)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaminaRegenRate")
	float StaminaRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxSpeed")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StunRate")
	float StunRate;
};
