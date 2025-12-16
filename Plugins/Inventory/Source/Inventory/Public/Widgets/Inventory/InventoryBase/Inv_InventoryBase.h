// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryBase.generated.h"

class UInv_HoverItem;
class UInv_ItemComponent;
/**
 * 子类：UInv_SpatialInventory
 */
UCLASS()
class INVENTORY_API UInv_InventoryBase : public UUserWidget
{
	GENERATED_BODY()
public:

	//检查是否有空间
	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent) const
	{
		return FInv_SlotAvailabilityResult();
	}

	virtual void OnItemHovered(UInv_InventoryItem* Item) {} //鼠标悬停在当前道具上

	virtual void OnItemUnHovered() {} //鼠标移动开了

	virtual bool HasHoverItem() const { return false; }

	virtual UInv_HoverItem* GetHoverItem() const { return nullptr; }

protected:

private:
	
};
