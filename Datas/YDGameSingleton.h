// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YDCharacterObjectData.h"
#include "YDCharacterStatData.h"
#include "YDItemInfoData.h"
#include "UObject/NoExportTypes.h"
#include "YDGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class YD_API UYDGameSingleton : public UObject
{
	GENERATED_BODY()
public:
	UYDGameSingleton();
	static UYDGameSingleton& Get();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Select")
	int32 CharacterSelectID =-1;
	
public:
	FORCEINLINE FYDCharacterObjectData GetCharacterObject(int32 InIndex) const
	{return CharacterObjectTable.IsValidIndex(InIndex-1) ? CharacterObjectTable[InIndex-1] : FYDCharacterObjectData(); }

	FORCEINLINE FYDItemInfoData GetItemData(int32 InIndex) const
	{ return ItemInfoDataTable.IsValidIndex(InIndex -1) ?ItemInfoDataTable[InIndex -1] :FYDItemInfoData();}
	
	FORCEINLINE FYDCharacterStatData
	GetCharacterStatData(int32 InIndex) const
	{ return CharacterStatTable.IsValidIndex(InIndex -1) ?CharacterStatTable[InIndex -1] :FYDCharacterStatData();}

	
	UPROPERTY()
	int32 CharacterObjectMaxIndex;

	UPROPERTY()
	int32 CharacterStatMaxIndex;

private:
	TArray<FYDCharacterObjectData> CharacterObjectTable;
	TArray<FYDItemInfoData> ItemInfoDataTable;
	TArray<FYDCharacterStatData> CharacterStatTable;

};
