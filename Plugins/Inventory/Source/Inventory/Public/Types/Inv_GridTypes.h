#pragma once

#include "Inv_GridTypes.generated.h"

class UInv_InventoryItem;
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

/**
 * 单个道具格子可用性
 * 包含信息如下：
 * 当前这个道具格子的Index
 * 当前这个道具格子可以填充多少道具，例如 当前这个道具格子放了一个可以堆叠10个的道具，那这里就是10
 * 当前这个道具格子是否有道具，如果有，我们只需要增加这个道具的堆叠数量就可以，如果没有，则需要创建一个这个道具的Widget显示在界面上
 */
USTRUCT()
struct FInv_SlotAvailability
{
	GENERATED_BODY()

	FInv_SlotAvailability() {};
	FInv_SlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {};

	int32 Index{INDEX_NONE};//当前这个道具格子的Index
	int32 AmountToFill{0};//当前这个道具格子可以填充多少道具
	bool bItemAtIndex{false};//当前这个道具格子是否有道具
	
};


/**
 * 槽位可用性结果
 * 包含信息如下：
 * 当前正在添加进背包的道具指针 TWeakObjectPtr<UInv_InventoryItem> Item
 * 当前正在添加的这个道具是否可以堆叠 bool bStackable{false};
 * 如果是可以堆叠的 那我们需要这个这个道具多少个一组 可以堆叠多少个 int32 TotalRoomToFill{0};
 * 如果是可以堆叠的，并且背包空间不足，我们需要知道还余下多少个放不进背包int32 Remainder{0}
 * 我们的槽位可用性结果应该包含这些的一个 T 数组
 */
USTRUCT()
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()
	
	FInv_SlotAvailabilityResult() {};

	TWeakObjectPtr<UInv_InventoryItem> Item;//当前正在添加进背包的道具指针

	bool bStackable{false};//当前正在添加的这个道具是否可以堆叠

	int32 TotalRoomToFill{0}; //可以堆叠的数量

	int32 Remainder{0}; //还余下多少个放不进背包

	TArray<FInv_SlotAvailability> SlotAvailabilities;//我们的槽位可用性结果应该包含这些的一个数组
	
	
};
