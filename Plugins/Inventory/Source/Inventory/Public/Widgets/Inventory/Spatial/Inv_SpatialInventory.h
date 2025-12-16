// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Inv_SpatialInventory.generated.h"

struct FGameplayTag;
class UInv_EquippedGridSlot;
class UInv_ItemDescription;
class UCanvasPanel;
class UButton;
class UWidgetSwitcher;
class UInv_InventoryGrid;

/**
 * 空间化背包界面类
 * 
 * 该类负责管理具有空间布局的背包系统UI，支持不同类型的物品分类显示。
 * 通过网格布局和切换器实现装备、消耗品、制作材料等不同类型物品的分类管理。
 * 
 * 主要功能：
 * - 提供HasRoomForItem方法检查是否有空间存放物品
 * - 通过WidgetSwitcher实现不同类型物品网格的切换显示
 * - 管理装备、消耗品、可制作物品三个分类的网格界面
 * - 提供按钮交互功能，支持用户切换查看不同类型的物品
 * 
 * 可用方法：
 * - HasRoomForItem: 检查背包是否有足够空间存放指定物品
 * - ShowEquippable: 显示装备类物品网格
 * - ShowConsumable: 显示消耗品类物品网格  
 * - ShowCraftable: 显示可制作物品网格
 * - SetActiveGrid: 激活指定的物品网格并禁用对应按钮
 * - DisableButton: 禁用指定按钮
 */
UCLASS()
class INVENTORY_API UInv_SpatialInventory : public UInv_InventoryBase
{
	GENERATED_BODY()
public:
	
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/**
	 * 有存放物品的空间
	 * @param ItemComponent 传入正在被拾取的道具ItemComponent 
	 * @return 返回一个FInv_SlotAvailabilityResult的拾取结果
	 */
	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent) const;

	virtual void OnItemHovered(UInv_InventoryItem* Item) override;

	virtual void OnItemUnHovered() override;

	virtual bool HasHoverItem() const override;

protected:

private:

	UPROPERTY()
	TArray<TObjectPtr<UInv_EquippedGridSlot>> EquippedGridSlots;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Equippable;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Consumable;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Craftable;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Equippable;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consumable;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Craftable;

	UPROPERTY(EditAnywhere, Category="属性设置")
	TSubclassOf<UInv_ItemDescription> ItemDescriptionClass;

	UPROPERTY()
	TObjectPtr<UInv_ItemDescription> ItemDescription;

	FTimerHandle DescriptionTimer;

	//设置道具描述控件的显示延迟时间
	UPROPERTY(EditAnywhere, Category="属性设置")
	float DescriptionTimerDelta{.5f};

	UInv_ItemDescription* GetItemDescription();

	UFUNCTION()
	void ShowEquippables();

	UFUNCTION()
	void ShowConsumable();

	UFUNCTION()
	void ShowCraftables();

	UFUNCTION()
	void EquippedGridSlotClicked(UInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag);

	
	//指定的按钮设置为禁用状态
	void DisableButton(UButton* Button);

	/**
	 * 设置指定的道具栏被激活（装备、消耗品、材料 其中一个），并且让指定的按钮设置为禁用状态，表示当前显示的是这个类型的道具栏
	 * @param Grid 指定的道具栏被激活（装备、消耗品、材料 其中一个）
	 * @param Button 指定的按钮设置为禁用状态
	 */
	void SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button);

	void SetItemDescriptionSizeAndPostion(UInv_ItemDescription* Description, UCanvasPanel* Canvas) const;

	TWeakObjectPtr<UInv_InventoryGrid> ActiveGrid;
	
};
