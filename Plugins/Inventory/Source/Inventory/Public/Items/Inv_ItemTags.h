// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

//道具分类Tag
namespace GameItems 
{
	//装备
	namespace Equipment
	{
		//武器
		namespace Weapons
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Axe)//斧头
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sword)//剑
		}

		//斗篷
		namespace Cloaks
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(RedCloak)
		}

		//面具
		namespace Masks
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(SteelMask)
		}
	}

	//消耗品
	namespace Consumables
	{
		//药水
		namespace Potions
		{
			//生命药水
			namespace Red
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Small)
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Large)
			}

			//法力药水
			namespace Blue
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Small)
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Large)
			}
		}
		
	}

	//材料
	namespace Craftables
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireFruit)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(LuminDaisy)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ScorchPetalBlossom)
	}
}




