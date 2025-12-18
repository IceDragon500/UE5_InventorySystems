// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"


/**
 * 
 */
namespace FragmentTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GridFragment) //网格片段
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IconFragment) //道具图标片段
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StackableFragment) //道具堆叠数量片段
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ConsumableFragment) //消耗品使用片段

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ItemNameFragment) //道具名称文字片段
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(PrimaryStatFragment) //主要属性文字片段

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ItemTypeFragment) //道具类型文字片段
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(FlavorTextFragment) //道具描述文字片段
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SellValueFragment) //道具售价文字片段
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(RequiredLevelFragment) //适用角色等级属性文字片段

	namespace StatMod
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatMod_1)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatMod_2)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatMod_3)
	}

}

