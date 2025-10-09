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
 * 包含单个背包格子的可用性信息:
 * Index当前道具格子的索引
 * AmountToFill当前道具格子可以填充的道具数量
 * bItemAtIndex当前道具格子是否已有道具
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

/**
 * 
 */
UENUM()
enum class EInv_TileQuadrant : uint8
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

/**
 * 瓦片参数
 * 用来保存实时的、鼠标指向每一格道具格子的参数
 * 包括这个格子在整个道具栏中的坐标TileCoordinats
 * 包括这个格子所在的象限TileQuadrant
 * 包括这个格子的索引TileIndex
 */
USTRUCT(BlueprintType)
struct FInv_TileParameters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="属性设置")
	FIntPoint TileCoordinats{}; //瓦片坐标信息

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="属性设置")
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="属性设置")
	EInv_TileQuadrant TileQuadrant{EInv_TileQuadrant::None};
	
};

// 重载==运算符 使用==可以判断两个结构体是否一致
inline bool operator==(const FInv_TileParameters& LHS, const FInv_TileParameters& RHS)
{
	return LHS.TileCoordinats == RHS.TileCoordinats && LHS.TileIndex == RHS.TileIndex && LHS.TileQuadrant == RHS.TileQuadrant;
}

USTRUCT()
struct FInv_SpaceQueryResult
{
	GENERATED_BODY()

	//如果查询的空间中没有物品，我们就返回true ,
	bool bHasSpace{false};

	//如果存在单个可交换物品，我将判定为有效
	TWeakObjectPtr<UInv_InventoryItem> ValidItem = nullptr;

	//有效物品的左上角索引（如果存在的话）
	int32 UpperLeftIndex{INDEX_NONE};
	
};


