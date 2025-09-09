// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Inv_SlottedItems.generated.h"

class UImage;
/**
 * 插槽物品：在UserWidget上的表示
 * 图标
 */
UCLASS()
class INVENTORY_API UInv_SlottedItems : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetIsStackable(bool bStackable) { bIsStackable = bStackable; }
	bool IsStackable() const { return bIsStackable; }
	
	UImage* GetImage() { return Image_Icon; }
	
	void SetGridIndex(int Index) { GridIndex = Index; }
	int32 GetGridIndex() const { return GridIndex; }
	
	void SetGridDiemsions(const FIntPoint& InDimensions) { GridDimensions = InDimensions; }
	FIntPoint GetGridDimensions() const { return GridDimensions; }

	void SetInventoryItem(UInv_InventoryItem* Item) { InventoryItem = Item; }
	UInv_InventoryItem* GetInventoryItem() const { return InventoryItem.Get(); }

	void SetImageBrush(const FSlateBrush& Brush) const;

protected:

private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Icon;

	//道具在背包中的Index序号
	int32 GridIndex{0};

	//自身的网格尺寸
	FIntPoint GridDimensions{0, 0};

	//道具类
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;

	//是否可以堆叠
	bool bIsStackable{false};
	
};
