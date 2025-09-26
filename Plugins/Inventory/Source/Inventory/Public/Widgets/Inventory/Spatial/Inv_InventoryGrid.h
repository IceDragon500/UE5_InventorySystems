// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Types/Inv_GridTypes.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Inv_InventoryGrid.generated.h"


struct FInv_GridFragment;
class UInv_SlottedItem;
class UInv_ItemComponent;
class UInv_InventoryComponent;
class UCanvasPanel;
class UInv_GridSlot;

/**
 * InventoryGrid 道具格子
 *
 * 用来处理道具格子的功能和显示
 *
 * 定义了一个公共的EInv_ItemCategory枚举，用来区分3中道具格：装备、消耗品、材料
 *
 * 在蓝图上使用一个Switch空间来切换3种道具分类格子
 *
 * 用来生成指定参数的道具格子，并且有对应的方法
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

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
	void SetSlottedItemImage(UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment,
	                         const FInv_ImageFragment* ImageFragment);

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
	
	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const;

	int32 DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const UInv_GridSlot* GirdSlot) const;

	int32 GetStackAmount(const UInv_GridSlot* GridSlot) const;

	UFUNCTION()
	void AddStacks(const FInv_SlotAvailabilityResult& Result);


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
};
