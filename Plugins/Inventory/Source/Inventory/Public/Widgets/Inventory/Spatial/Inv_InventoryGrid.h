// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"

/**
 * InventoryGrid 道具格子
 *
 * 用来处理道具格子的功能和显示
 *
 * 定义了一个公共的EInv_ItemCategory枚举，用来区分3中道具格：装备、消耗品、材料
 *
 * 在蓝图上使用一个Switch空间来切换3种道具分类格子
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

protected:

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Inventory")
	EInv_ItemCategory ItemCategory;
};
