#pragma once
#include "NativeGameplayTags.h"

namespace YDGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Attacking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_GeneralAction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Death);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Hit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Blocking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Parrying);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Parried);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Stunned);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_UsePotion);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_TrowItem);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_Heavy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_Light);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_Running);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_Special);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_Air);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_Equip);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_Unequip);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_HitReaction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_BlockingHit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_ParriedHit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_KnockBackHit);


	
}
