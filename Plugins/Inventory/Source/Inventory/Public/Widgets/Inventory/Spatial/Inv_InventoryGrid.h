// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Types/Inv_GridTypes.h"
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

	//获取道具栏的类型
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	//通过传入道具的item component来判断道具栏是否还有空间
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* ItemComponent);

	
	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);


protected:

private:

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	//构造道具格子
	void ConstructGrid();

	//通过传入道具的item来判断道具栏是否还有空间
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& ItemManifest);

	//接收我们的槽位可用性结果以及物品本身，然后由它来处理后续事宜（Indices : index的复数）
	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem);
	
	/**
	 * 检查添加的物品类别是否与此物品栏的物品类别匹配
	 * @param Item 被添加的物品
	 * @return
	 */
	bool MatchesCategory(const UInv_InventoryItem* Item) const;

	FVector2D GetDrawSize(const FInv_GridFragment* GridFragment) const;

	void SetSlottedItemImage(UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment);

	void AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount);

	UInv_SlottedItem* CreateSlottedItem(UInv_InventoryItem* Item, const bool bStackable, const int32 StackAmount, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment, const int32 Index);

	void AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment, UInv_SlottedItem* SlottedItem) const;

	//道具栏的类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="属性设置")
	EInv_ItemCategory ItemCategory {EInv_ItemCategory::Equippable};

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
	int32 Rows { 1 };//道具栏的行 从上到下有多少行

	UPROPERTY(EditAnywhere, Category="属性设置")
	int32 Columns { 1 };//道具栏的列 从左到右有多少列
	
	UPROPERTY(EditAnywhere, Category="属性设置")
	float TileSize { 20.f };//单个格子的大小 - 边长


};