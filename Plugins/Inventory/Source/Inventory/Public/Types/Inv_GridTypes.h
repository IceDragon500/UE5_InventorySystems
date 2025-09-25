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
	Equippable, //装备
	Consumable, //消耗品
	Craftable, //材料
	None, //无
};


/**
 * 单个道具格子可用性
 * 
 * 包含单个背包格子的可用性信息，用于描述该格子是否可以放置道具、
 * 可以放置多少数量的道具以及该格子当前是否已有道具等信息
 */
USTRUCT()
struct FInv_SlotAvailability
{
	GENERATED_BODY()

	/**
	 * 默认构造函数
	 */
	FInv_SlotAvailability() {};
	
	/**
	 * 带参数构造函数
	 * 
	 * @param ItemIndex 格子索引
	 * @param Room 可填充的道具数量
	 * @param bHasItem 该格子是否已有道具
	 */
	FInv_SlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {};

	/**
	 * 当前道具格子的索引
	 * 用于标识背包中具体是哪一个格子
	 */
	int32 Index{INDEX_NONE};
	
	/**
	 * 当前道具格子可以填充的道具数量
	 * 例如：格子中已有一个可堆叠10个的道具，当前有3个，则此值为7
	 */
	int32 AmountToFill{0};
	
	/**
	 * 当前道具格子是否已有道具
	 * true表示格子中已有道具（可堆叠情况），false表示格子为空
	 */
	bool bItemAtIndex{false};
	
};

/**
 * 道具槽位可用性结果
 * 
 * 用于存储尝试向背包添加道具时的槽位可用性分析结果，
 * 包含道具信息、堆叠能力、可用空间以及剩余无法放置的数量等信息
 */
USTRUCT()
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()
	
	/**
	 * 默认构造函数
	 */
	FInv_SlotAvailabilityResult() {};

	/**
	 * 当前正在添加进背包的道具指针
	 * 用于引用需要添加到背包中的具体道具对象
	 */
	TWeakObjectPtr<UInv_InventoryItem> Item;

	/**
	 * 当前正在添加的这个道具是否可以堆叠
	 * true表示可以堆叠，false表示不可堆叠
	 */
	bool bStackable{false};

	/**
	 * 可以堆叠的数量
	 * 表示当前道具在各个可用槽位中总共可以放置的数量
	 */
	int32 TotalRoomToFill{0}; 

	/**
	 * 还余下多少个放不进背包
	 * 当尝试添加的道具数量超过可用空间时，此值表示无法放置的剩余数量
	 */
	int32 Remainder{0}; 

	/**
	 * 槽位可用性数组
	 * 包含所有可用槽位的详细信息，每个元素描述一个槽位的索引、可填充数量和是否有道具等信息
	 */
	TArray<FInv_SlotAvailability> SlotAvailabilities;
	
	
};

