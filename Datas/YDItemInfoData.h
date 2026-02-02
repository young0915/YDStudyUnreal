#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "YDItemInfoData.generated.h"


USTRUCT(BlueprintType)
struct FYDItemInfoData :public FTableRowBase
{
	GENERATED_BODY()

	FYDItemInfoData() : Attack(0.0f), StaminaDamage(0.0f), Defense(0.0f), HeavyAttack(0.0),
	HeavyStamina(0.0f), SpecialAttack(0.0f),SpecialStamina(0.0f),TraceRadius(0.0f), Health(0.0f), ItemName(TEXT(""))
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Attack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaminaDamage")
	float StaminaDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeavyAttack")
	float HeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeavyStamina")
	float HeavyStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack")
	float SpecialAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialStamina")
	float SpecialStamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceRadius")
	float TraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemName")
	FString ItemName;



};
