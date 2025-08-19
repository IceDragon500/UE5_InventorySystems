// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Inv_SpatialInventory.generated.h"

class UButton;
class UWidgetSwitcher;
class UInv_InventoryGrid;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_SpatialInventory : public UInv_InventoryBase
{
	GENERATED_BODY()
public:
	
	virtual void NativeOnInitialized() override;

protected:

private:
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

	UFUNCTION()
	void ShowEquippable();

	UFUNCTION()
	void ShowConsumable();

	UFUNCTION()
	void ShowCraftable();

	
	//指定的按钮设置为禁用状态
	void DisableButton(UButton* Button);

	/**
	 * 设置指定的道具栏被激活（装备、消耗品、材料 其中一个），并且让指定的按钮设置为禁用状态，表示当前显示的是这个类型的道具栏
	 * @param Grid 指定的道具栏被激活（装备、消耗品、材料 其中一个）
	 * @param Button 指定的按钮设置为禁用状态
	 */
	void SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button);
	
};
