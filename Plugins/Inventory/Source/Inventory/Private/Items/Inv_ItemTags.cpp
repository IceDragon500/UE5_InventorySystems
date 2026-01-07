// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inv_ItemTags.h"


//道具分类Tag
namespace GameItems 
{
	//装备
	namespace Equipment
	{
		//武器
		namespace Weapons
		{
			UE_DEFINE_GAMEPLAY_TAG(Axe, "GameItems.Equipment.Weapons.Axe")//斧头
			UE_DEFINE_GAMEPLAY_TAG(Sword, "GameItems.Equipment.Weapons.Sword")//剑
		}

		//斗篷
		namespace Cloaks
		{
			UE_DEFINE_GAMEPLAY_TAG(RedCloak, "GameItems.Equipment.Cloaks.RedCloak")
			UE_DEFINE_GAMEPLAY_TAG(BlueCloak, "GameItems.Equipment.Cloaks.BlueCloak")
			UE_DEFINE_GAMEPLAY_TAG(GreenCloak, "GameItems.Equipment.Cloaks.GreenCloak")
			UE_DEFINE_GAMEPLAY_TAG(YellowCloak, "GameItems.Equipment.Cloaks.YellowCloak")
		}

		//面具
		namespace Masks
		{
			UE_DEFINE_GAMEPLAY_TAG(BlueMask, "GameItems.Equipment.Masks.BlueMask")
			UE_DEFINE_GAMEPLAY_TAG(GreenMask, "GameItems.Equipment.Masks.GreenMask")
			UE_DEFINE_GAMEPLAY_TAG(RedMask, "GameItems.Equipment.Masks.RedMask")
			UE_DEFINE_GAMEPLAY_TAG(YellowMask, "GameItems.Equipment.Masks.YellowMask")
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
				UE_DEFINE_GAMEPLAY_TAG(Small, "GameItems.Consumables.Potions.Red.Small")
				UE_DEFINE_GAMEPLAY_TAG(Large, "GameItems.Consumables.Potions.Red.Large")
			}

			//法力药水
			namespace Blue
			{
				UE_DEFINE_GAMEPLAY_TAG(Small, "GameItems.Consumables.Potions.Blue.Small")
				UE_DEFINE_GAMEPLAY_TAG(Large, "GameItems.Consumables.Potions.Blue.Large")
			}
		}
		
	}

	//材料
	namespace Craftables
	{
		UE_DEFINE_GAMEPLAY_TAG(FireFruit, "GameItems.Craftables.FireFruit")
		UE_DEFINE_GAMEPLAY_TAG(LuminDaisy, "GameItems.Craftables.LuminDaisy")
		UE_DEFINE_GAMEPLAY_TAG(ScorchPetalBlossom, "GameItems.Craftables.ScorchPetalBlossom")
	}
}
