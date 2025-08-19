#pragma once

#include "Inv_GridTypes.generated.h"

/**
 * 定义道具类型
 * 
 * Equippable 装备
 * 
 * Consumable 消耗品
 * 
 * Craftable 材料
 * 
 * None 无
 */
UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8
{
	Equippable,
	Consumable,
	Craftable,
	None,
};