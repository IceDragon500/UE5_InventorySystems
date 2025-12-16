// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_GridSlot.h"
#include "Inv_EquippedGridSlot.generated.h"

/**
 * 定义为装备栏
 * 用来处理类行为“装备”的道具，添加在这个道具栏中的相关事件
 * 我们需要在角色身上显示它
 * 需要应用任何装备时或在卸下装备时，增益效果、减益效果以及各类效果都会移除
 * 当我们点击已装备的网格槽时，我们需要广播该委托，以便更重要的类能够响应它
 * NativeOnMouseEnter 鼠标何时进入
 * NativeOnMouseLeave 鼠标何时离开
 * NativeOnMouseButtonDown 鼠标何时点击
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedGridSlotClicked, UInv_EquippedGridSlot*, GridSlot, const FGameplayTag&, EquipmentTypeTag);

UCLASS()
class INVENTORY_API UInv_EquippedGridSlot : public UInv_GridSlot
{
	GENERATED_BODY()
public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	FEquippedGridSlotClicked EquippedGridSlotClicked;

protected:

private:

	//装备类型的Tag
	UPROPERTY(EditAnywhere, Category="属性设置")
	FGameplayTag EquipmentTypeTag;
};
