// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UImage;

/**
 * 道具栏格子不同的状态，会对应不同的背景图片资源
 */
UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8
{
	Unoccupied, //空
	Occupied, //占用
	Selected, //选中
	GrayedOut //显示为灰色
};

/**
 * 这里是单个的道具格子
 */
UCLASS()
class INVENTORY_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetTileIndex(int32 Index) { TileIndex = Index; }
	int32 GetTileIndex() const { return TileIndex; }

	EInv_GridSlotState GetSlotState() const { return GridSlotState; }
	void SetSlotState(EInv_GridSlotState NewState) { GridSlotState = NewState; }

	TWeakObjectPtr<UInv_InventoryItem> GetInventoryItem() const { return InventoryItem; }
	void SetInventoryItem(TWeakObjectPtr<UInv_InventoryItem> Item);

	void SetUpperLeftIndex(int32 Index) { UpperLeftIndex = Index; }
	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }

	void SetStackCount(int32 Count) { StackCount = Count; }
	int32 GetStackCount() const { return StackCount; }

	void SetAvailable(bool bAble) { bAvailable = bAble; }
	bool GetAvailable() const { return bAvailable; }

	void SetUnoccupiedTexture();
	void SetOccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

protected:

private:
	
	int32 TileIndex {0};//单个格子的索引Index
	int32 StackCount{0};//单个格子的堆叠数量
	int32 UpperLeftIndex{INDEX_NONE};//任何给定物品的左上角索引
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;//这个格子指向的具体道具的弱指针
	bool bAvailable{false};//是否可用

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditAnywhere, Category="属性设置")
	FSlateBrush Brush_Unoccupied;//显示为默认的图片

	UPROPERTY(EditAnywhere, Category="属性设置")
	FSlateBrush Brush_Occupied;//显示为已占用的背景图片

	UPROPERTY(EditAnywhere, Category="属性设置")
	FSlateBrush Brush_Selected;//显示为已选中的背景图片

	UPROPERTY(EditAnywhere, Category="属性设置")
	FSlateBrush Brush_GrayedOut;//显示为灰色的背景图片

	//当前网格的状态
	EInv_GridSlotState GridSlotState{EInv_GridSlotState::Unoccupied};


};
