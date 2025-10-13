// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Types/Inv_GridTypes.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Inv_InventoryGrid.generated.h"


class UInv_HoverItem;
struct FInv_GridFragment;
class UInv_SlottedItem;
class UInv_ItemComponent;
class UInv_InventoryComponent;
class UCanvasPanel;
class UInv_GridSlot;

/**
 * InventoryGrid 道具格子管理类
 * 
 * 该类负责管理背包系统中的网格布局，处理物品在网格中的放置、显示和交互。
 * 支持不同尺寸的物品放置，提供空间检测、物品分类管理等功能。
 * 
 * 主要功能：
 * - 管理指定分类的物品网格（装备、消耗品、材料等）
 * - 处理物品的空间布局和放置逻辑
 * - 创建和维护网格槽位及物品显示
 * - 支持物品堆叠和空间检测
 * 
 * 可用方法：
 * - HasRoomForItem: 检查是否有足够空间放置指定物品
 * - AddItem: 添加物品到网格中
 * - AddStacks: 添加堆叠物品
 * - GetItemCategory: 获取当前网格的物品分类
 * - ConstructGrid: 构造网格系统
 * - MatchesCategory: 检查物品是否匹配当前网格分类
 * - IsInGridBounds: 检查索引是否在网格边界内
 * 
 * 核心私有方法：
 * - HasRoomAtIndex: 检查指定位置是否有足够空间
 * - CheckSlotConstraints: 检查槽位约束条件
 * - CreateSlottedItem: 创建槽位物品实例
 * - UpdateGridSlots: 更新网格槽位状态
 * - AddItemToIndices: 将物品添加到指定索引位置
 * - SetSlottedItemImage: 设置槽位物品图像
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* ItemComponent);

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);

protected:

private:
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	/**
	 * 构造网格系统，根据行数和列数创建对应的网格槽位
	 * 初始化网格布局并创建所有网格槽位实例
	 */
	void ConstructGrid();

	/**
	 * 检查是否有足够空间放置指定的物品
	 * 
	 * @param Item 需要检查的物品组件
	 * @return 返回槽位可用性结果，包含是否可放置以及放置位置信息
	 */
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);

	/**
	 * 检查是否有足够空间放置指定的物品清单
	 * 
	 * @param ItemManifest 物品清单信息
	 * @return 返回槽位可用性结果，包含是否可放置以及放置位置信息
	 */
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& ItemManifest);

	/**
	 * 将物品添加到指定的网格索引位置
	 * 
	 * @param Result 槽位可用性结果，包含可放置的位置信息
	 * @param NewItem 需要添加的新物品
	 */
	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem);

	/**
	 * 检查物品是否匹配当前网格的分类
	 * 
	 * @param Item 需要检查的物品
	 * @return 如果物品分类匹配返回true，否则返回false
	 */
	bool MatchesCategory(const UInv_InventoryItem* Item) const;

	/**
	 * 获取网格片段的绘制尺寸
	 * 
	 * @param GridFragment 网格片段信息
	 * @return 返回绘制尺寸的二维向量
	 */
	FVector2D GetDrawSize(const FInv_GridFragment* GridFragment) const;

	/**
	 * 设置槽位物品的图像
	 * 
	 * @param SlottedItem 槽位物品实例
	 * @param GridFragment 网格片段信息
	 * @param ImageFragment 图像片段信息
	 */
	void SetSlottedItemImage(const UInv_SlottedItem* SlottedItem,
							const FInv_GridFragment* GridFragment,
							const FInv_ImageFragment* ImageFragment) const;

	/**
	 * 在指定索引位置添加物品
	 * 
	 * @param Item 需要添加的物品
	 * @param Index 网格索引位置
	 * @param bStackable 是否可堆叠
	 * @param StackAmount 堆叠数量
	 */
	void AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount);

	/**
	 * 创建槽位物品实例
	 * 
	 * @param Item 物品对象
	 * @param bStackable 是否可堆叠
	 * @param StackAmount 堆叠数量
	 * @param GridFragment 网格片段信息
	 * @param ImageFragment 图像片段信息
	 * @param Index 网格索引位置
	 * @return 创建的槽位物品实例
	 */
	UInv_SlottedItem* CreateSlottedItem(UInv_InventoryItem* Item, const bool bStackable, const int32 StackAmount,
	                                    const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment,
	                                    const int32 Index);

	/**
	 * 将槽位物品添加到画布中
	 * 
	 * @param Index 网格索引位置
	 * @param GridFragment 网格片段信息
	 * @param SlottedItem 槽位物品实例
	 */
	void AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment,
	                            UInv_SlottedItem* SlottedItem) const;

	/**
	 * 更新网格槽位状态
	 * 
	 * @param NewItem 新添加的物品
	 * @param Index 网格索引位置
	 * @param bStackableItem 是否为可堆叠物品
	 * @param StackAmount 堆叠数量
	 */
	void UpdateGridSlots(UInv_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount);

	/**
	 * 检查索引是否已被占用
	 * 
	 * @param CheckedIndices 已检查的索引集合
	 * @param Index 需要检查的索引
	 * @return 如果索引已被占用返回true，否则返回false
	 */
	bool IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const;

	/**
	 * 检查指定网格槽位是否有足够空间放置指定尺寸的物品
	 * 
	 * @param GridSlot 网格槽位
	 * @param Dimensions 物品尺寸
	 * @param CheckedIndices 已检查的索引集合
	 * @param OutTentativelyClaimed 输出参数，临时占用的索引集合
	 * @param ItemType 物品类型标签，用于检查物品类型约束
	 * @param MaxStackSize 验证是否有足够的空间来容纳物品的堆叠数量
	 * @return 如果有足够空间返回true，否则返回false
	 */
	bool HasRoomAtIndex(const UInv_GridSlot* GridSlot,
	                    const FIntPoint& Dimensions,
	                    const TSet<int32>& CheckedIndices,
	                    TSet<int32>& OutTentativelyClaimed,
	                    const FGameplayTag& ItemType,
	                    const int32 MaxStackSize);

	/**
	 * 检查网格槽位的约束条件，验证指定位置是否可以放置特定类型的物品
	 * 
	 * @param GridSlot 主网格槽，用于检查约束条件
	 * @param SubGridSlot 子网格槽，用于与主网格槽进行约束比较
	 * @param CheckedIndices 已检查的索引集合，避免重复检查
	 * @param OutTentativelyClaimed 输出参数，临时占用的索引集合
	 * @param ItemType 物品类型标签，用于检查物品类型约束
	 * @param MaxStackSize 验证是否有足够的空间来容纳物品的堆叠数量
	 * @return 如果满足所有约束条件返回true，否则返回false
	 */
	bool CheckSlotConstraints(const UInv_GridSlot* GridSlot,
	                          const UInv_GridSlot* SubGridSlot,
	                          const TSet<int32>& CheckedIndices,
	                          TSet<int32>& OutTentativelyClaimed,
	                          const FGameplayTag& ItemType,
	                          const int32 MaxStackSize) const;


	/**
	 * 获取物品清单中的物品尺寸信息
	 * 
	 * @param ItemManifest 物品清单
	 * @return 返回物品在网格中的尺寸(宽x高)
	 */
	FIntPoint GetItemDimensions(const FInv_ItemManifest& ItemManifest) const;

	/**
	 * 检查指定网格槽是否包含有效的物品
	 * 
	 * @param GridSlot 需要检查的网格槽
	 * @return 如果网格槽包含有效物品返回true，否则返回false
	 */
	bool HasValidItem(const UInv_GridSlot* GridSlot) const;


	/**
	 * 判断给定的网格槽是否是子网格槽的左上角槽位
	 *
	 * @param GridSlot 主网格槽，用于检查是否为左上角位置
	 * @param SubGridSlot 子网格槽，用于与主网格槽进行位置比较
	 * @return 如果GridSlot是SubGridSlot的左上角槽位则返回true，否则返回false
	 */
	bool IsUpperLeftSlot(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot) const;

	/**
	 * 检查子物品的类型是否与指定的物品类型标签匹配
	 * 
	 * @param SubItem 需要检查的子物品
	 * @param ItemType 目标物品类型标签
	 * @return 如果子物品类型与目标类型匹配返回true，否则返回false
	 */
	bool DoesItemTypeMatch(const UInv_InventoryItem* SubItem, const FGameplayTag& ItemType) const;

	/**
	 * 检查指定起始索引和物品尺寸是否在网格边界内
	 * 
	 * @param StartIndex 起始网格索引位置
	 * @param ItemDimensions 物品占用的网格尺寸
	 * @return 如果物品可以完全放置在网格内返回true，否则返回false
	 */
	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const;

	/**
	 * 确定在指定槽位中需要填充的物品数量
	 * 
	 * @param bStackable 物品是否可堆叠
	 * @param MaxStackSize 物品最大堆叠数量
	 * @param AmountToFill 需要填充的总数量
	 * @param GirdSlot 目标网格槽位
	 * @return 实际需要在该槽位填充的数量
	 */
	int32 DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill,
	                                 const UInv_GridSlot* GirdSlot) const;

	/**
	 * 获取指定网格槽位的当前堆叠数量
	 * 
	 * @param GridSlot 需要查询的网格槽位
	 * @return 该槽位当前的堆叠数量
	 */
	int32 GetStackAmount(const UInv_GridSlot* GridSlot) const;

	/**
	 * 检查鼠标事件是否为右键点击
	 * 
	 * @param MouseEvent 鼠标事件数据
	 * @return 如果是右键点击返回true，否则返回false
	 */
	bool IsRightClick(const FPointerEvent& MouseEvent) const;

	/**
	 * 检查鼠标事件是否为左键点击
	 * 
	 * @param MouseEvent 鼠标事件数据
	 * @return 如果是左键点击返回true，否则返回false
	 */
	bool IsLeftClick(const FPointerEvent& MouseEvent) const;


	/**
	 * 鼠标左键点击物品栏中的道具，将物品“拾取”起来
	 * @param ClickedInventoryItem 被点击到的道具
	 * @param GridIndex 被电击到的道具格子索引
	 */
	void PickUp(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex);

	/**
	 * 创建被鼠标点击并且“悬浮”在鼠标上的道具
	 * @param InventoryItem 被鼠标点击的道具
	 */
	void AssignHoverItem(UInv_InventoryItem* InventoryItem);

	/**
	 * 创建被鼠标点击并且“悬浮”在鼠标上的道具
	 * 如果我们仅有一个网格索引而没有前一个索引，比如拾取物品时的情况，那么我采取的做法是直接将网格索引和前一个网格索引都赋值为同一索引
	 * @param InventoryItem 被鼠标点击的道具
	 * @param GridIndex 被点击道具的网格索引
	 * @param PreviousGridIndex 被点击道具前一个的索引
	 */
	void AssignHoverItem(UInv_InventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex);

	void RemoveItemFromGrid(UInv_InventoryItem* InventoryItem, const int32 GridIndex);

	void UpdateTileParameters(const FVector2D CanvasPosition, const FVector2D MousePosition);

	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	
	EInv_TileQuadrant CalculateTileQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;

	void OnTileParametersUpdate(const FInv_TileParameters& Parameters);

	UFUNCTION()
	void AddStacks(const FInv_SlotAvailabilityResult& Result);

	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	FIntPoint CalculateStartingCoordinate(const FIntPoint& Coordinate, const FIntPoint& Dimensions, EInv_TileQuadrant Quadrant) const;

	FInv_SpaceQueryResult CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions);

	bool CursorExitedCanvas(const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& Location);

	void HighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	
	void UnHighlightSlots(const int32 Index, const FIntPoint& Dimensions);

	void ChangeHoverType(const int32 Index, const FIntPoint& Dimensions, EInv_GridSlotState GridSlotState);


	//道具栏的类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="属性设置")
	EInv_ItemCategory ItemCategory{EInv_ItemCategory::Equippable};

	//道具格子数组
	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category="属性设置")
	TSubclassOf<UInv_GridSlot> GridSlotClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	//设置生成道具的UserWidget类
	UPROPERTY(EditAnywhere, Category="属性设置")
	TSubclassOf<UInv_SlottedItem> SlottedItemsClass;

	//建立网格数组的索引和实机的槽位物品之间的映射关系
	UPROPERTY()
	TMap<int32, TObjectPtr<UInv_SlottedItem>> SlottedItemMap;

	UPROPERTY(EditAnywhere, Category="属性设置")
	int32 Rows{1}; //道具栏的行 从上到下有多少行

	UPROPERTY(EditAnywhere, Category="属性设置")
	int32 Columns{1}; //道具栏的列 从左到右有多少列

	UPROPERTY(EditAnywhere, Category="属性设置")
	float TileSize{20.f}; //单个格子的大小 - 边长

	UPROPERTY(EditAnywhere, Category="属性设置")
	TSubclassOf<UInv_HoverItem> HoverItemClass; //用来显示鼠标悬停的Widget组件类

	UPROPERTY()
	TObjectPtr<UInv_HoverItem> HoverItem; //被创建的鼠标悬停的Widget组件

	//保存当前帧下，鼠标指向道具格子的信息
	FInv_TileParameters TileParameters;

	//上一帧，鼠标指向道具格子的信息
	FInv_TileParameters LastTileParameters;

	//当我们在有效位置点击网格时物品将被放置的位置
	int32 ItemDropIndex{INDEX_NONE};

	FInv_SpaceQueryResult CurrentQueryResult;

	bool bMouseWithinCanvas{false};
	bool bLastMouseWithinCanvas{false};

	int32 LastHighlightedIndex{INDEX_NONE};
	FIntPoint LastHighlightedDimensions{-1, -1};
};

