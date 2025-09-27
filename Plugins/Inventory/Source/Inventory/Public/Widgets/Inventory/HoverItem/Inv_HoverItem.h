// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HoverItem.generated.h"

struct FGameplayTag;
class UTextBlock;
class UInv_InventoryItem;
class UImage;
/**
 * 用来显示道具栏中的道具被鼠标点击时，显示在鼠标上的控件
 * 显示鼠标光标处的图标，并在必要时显示状态计数
 */
UCLASS()
class INVENTORY_API UInv_HoverItem : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetImageBrush(const FSlateBrush& Brush) const;//设置图标
	void UpdateStackCount(const int32 Count) const;//更新数量

	FGameplayTag GetItemType() const;
	int32 GetStackCount() const { return StackCount;}
	
	bool IsStackable() const { return bIsStackable; }
	void SetIsStackable(bool bStacks);
	
	int32 GetPreviousGridIndex() const { return PreviousGridIndex; }
	void SetPreviousGridIndex(int32 Index) { PreviousGridIndex = Index;}
	
	FIntPoint GetGridDimensions() const { return GridDimensions; }
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions;}

	UInv_InventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UInv_InventoryItem* Item);

protected:

	
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon; //跟随鼠标移动的图片资源

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;//如果堆叠的道具，则显示数量

	int32 PreviousGridIndex{0};//被点起来的道具，所在的网格索引

	FIntPoint GridDimensions{1,1};//被点起来的道具，自身的网格尺寸

	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;//被点起来的道具，自身的指针
	//使用弱引用符合UI组件不拥有核心数据对象的设计原则

	bool bIsStackable{false}; //被点起来的道具，是否是可以被堆叠的

	int32 StackCount{0};//被点起来的道具，当前的数量
};
