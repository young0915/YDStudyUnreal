#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "YDCharacterObjectData.generated.h"


USTRUCT(BlueprintType)
struct FYDCharacterObjectData : public FTableRowBase
{
	GENERATED_BODY()

	FYDCharacterObjectData() : CharacterMeshPath(TEXT("")), HairPath(TEXT("")), HairPositionX(0.0f), HairPositionY(0.0f), HairPositionZ(0.0f),
				 HairRotationPitch(0.0f), HairRotationYaw(0.0f), HairRotationRoll(0.0f)
	{
	}

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterMeshPath")
	FSoftObjectPath CharacterMeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairPath")
	FSoftObjectPath HairPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairPositionX")
	float HairPositionX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairPositionY")
	float HairPositionY;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairPositionZ")
	float HairPositionZ;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairRotationPitch")
	float HairRotationPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairRotationYaw")
	float HairRotationYaw;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairRotationRoll")
	float HairRotationRoll;
	
	


};
