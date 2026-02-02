// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/YDGameSingleton.h"


UYDGameSingleton::UYDGameSingleton()
{
	CharacterSelectID = -1;

	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterObjectDataTable(
		TEXT("/Script/Engine.DataTable'/Game/Game/InfoData/DT_CharacterObjectTable.DT_CharacterObjectTable'"));
	if (CharacterObjectDataTable.Object)
	{
		const UDataTable* DataTable = CharacterObjectDataTable.Object;
		check(DataTable->GetRowMap().Num()>0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterObjectTable,
						[](uint8* Value)
						{
							return *reinterpret_cast<FYDCharacterObjectData*>(Value);
						});
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponDataTableRef(
		TEXT("/Script/Engine.DataTable'/Game/Game/InfoData/DT_ItemInfoTable.DT_ItemInfoTable'"));
	if (WeaponDataTableRef.Object)
	{
		const UDataTable* DataTable = WeaponDataTableRef.Object;
		check(DataTable->GetRowMap().Num()>0);
		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, ItemInfoDataTable,

						[](uint8* Value)
						{
							return *reinterpret_cast<FYDItemInfoData*>(Value);
						});
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterStatTableRef(
		TEXT("/Script/Engine.DataTable'/Game/Game/InfoData/DT_StatTable.DT_StatTable'"));
	if (CharacterStatTableRef.Object)
	{
		const UDataTable* DataTable = CharacterStatTableRef.Object;
		check(DataTable->GetRowMap().Num()>0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,

						[](uint8* Value)
						{
							return *reinterpret_cast<FYDCharacterStatData*>(Value);
						});
	}

	CharacterObjectMaxIndex = CharacterObjectTable.Num();
	CharacterStatMaxIndex = CharacterStatTable.Num();
}

UYDGameSingleton& UYDGameSingleton::Get()
{
	UYDGameSingleton* Singleton = CastChecked<UYDGameSingleton>(GEngine->GameSingleton);

	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Display, TEXT("Invaliad Game Singleton"));
	return *NewObject<UYDGameSingleton>();
}
