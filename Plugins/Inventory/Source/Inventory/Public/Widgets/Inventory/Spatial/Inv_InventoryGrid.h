// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"

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

protected:

private:

	//构造道具格子
	void ConstructGrid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Inventory")
	EInv_ItemCategory ItemCategory;

	//道具格子数组
	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category="属性设置")
	TSubclassOf<UInv_GridSlot> GridSlotClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category="属性设置")
	int32 Rows { 1 };//行

	UPROPERTY(EditAnywhere, Category="属性设置")
	int32 Columns { 1 };//列
	
	UPROPERTY(EditAnywhere, Category="属性设置")
	float TileSize { 20.f };//单个格子的大小 - 边长
};
