// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/YDMontageActionData.h"

UAnimMontage* UYDMontageActionData::GetMontageForTag(const FGameplayTag& Tag, const int32 Index)
{
	if (MontageGroupsMap.Contains(Tag))
	{
		const FYDMontageGroup& MontageGroup = MontageGroupsMap[Tag];

		if (MontageGroup.Animations.Num() > 0 && MontageGroup.Animations.Num() > Index)
		{
			return MontageGroup.Animations[Index];
		}
	}

	return nullptr;
}

UAnimMontage* UYDMontageActionData::GetRandomMontageForTag(const FGameplayTag& Tag) const
{
	if (MontageGroupsMap.Contains(Tag))
	{
		const FYDMontageGroup& MontageGroup = MontageGroupsMap[Tag];

		const int32 Index = FMath::RandRange(0, MontageGroup.Animations.Num() - 1);

		return MontageGroup.Animations[Index];
	}
	
	return nullptr;
}
