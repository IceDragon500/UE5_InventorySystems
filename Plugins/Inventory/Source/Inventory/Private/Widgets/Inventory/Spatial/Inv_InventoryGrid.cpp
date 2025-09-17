// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &UInv_InventoryGrid::AddItem);
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_InventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& ItemManifest)
{
	/* 42讲注释掉了这些内容
	FInv_SlotAvailabilityResult Result;
	
	Result.TotalRoomToFill = 7;
	Result.bStackable = true;

	FInv_SlotAvailability SlotAvailability;
	SlotAvailability.AmountToFill = 2;
	SlotAvailability.Index = 0;
	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability));

	FInv_SlotAvailability SlotAvailability2;
	SlotAvailability2.AmountToFill = 5;
	SlotAvailability2.Index = 1;
	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability2));

	//MoveTemp会将引用强制转换为右值引用。这是UE的std::move的等效函数
	//除了当传递右值或const对象时它不会编译，因为我们更希望MoveTemp没有作用时被告知。
	*/

	FInv_SlotAvailabilityResult Result;

	//确定该物品是否可堆叠
	//如果那里有一个物品且不可堆叠，那么这个索引就不可用
	//如果那里有一个物品且可堆叠，那么我们必须检查该物品是否与我们尝试放置的物品是什么
	const FInv_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfType<FInv_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr;

	//如果它是可堆叠的，我们需要确定要添加多少个堆叠
	//如果拾取的数量超过了堆叠的数量，我们就必须遍历库存中的每个网格槽位进行检查
	//但在最坏的情况下，我们不得不检查每一个槽位
	const int32 MaxStackSize = StackableFragment ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountTolFill = StackableFragment ? StackableFragment->GetStackCount() : 1;

	//对于每个网格槽位，我们都需要执行一些操作 For each Grid Slot:
		//如果没有更多需要填充的，就提前跳出，或者简单地说提前退出循环
		//这个索引是否已被占用
		//如果这是一个有效的槽位，物品能否放得下,是在检查那些网格尺寸，即物品占用了多少个方格 它是否超出了网格的边界
		//这个索引位置有空位吗 是否有其他物品挡在路上
		//检查其他任何重要条件 ForEach2D 遍历一个二维范围内的方格(例如一个披风占据了2x3 6个格子，我们需要再进行一个循环来遍历这6个格子是否可用)
			//索引是否已被声明
			//拥有有效物品
			//如果存在有效物品，我们可以问这个物品是否与我们试图添加的物品类型相同
			//如果是的话，这是可堆叠的物品吗
			//如果可堆叠的槽位已经达到最大尺寸，或者我应该说已经达到最大堆叠尺寸，那么它是否处于最大容量
		//要填充多少
		//更新数量 剩余待填充
	
	//当我们遍历完每个槽位后，剩余的数量是多少
	
	return Result;
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	//UE_LOG(LogTemp, Warning, TEXT("InventoryGrid::AddItem 成功"));

	FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);

	//TODO : 接下来的任务是创建一个用于显示物品图标的控件，并将其添加到网格的正确位置
	AddItemToIndices(Result, Item);
}

void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem)
{

	for (const auto& Available : Result.SlotAvailabilities)
	{
		AddItemAtIndex(NewItem, Available.Index, Result.bStackable, Available.AmountToFill);
		UpdateGridSlots(NewItem, Available.Index, Result.bStackable, Available.AmountToFill);
	}
	
}

void UInv_InventoryGrid::AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount)
{
	//设定获取网格片段的功能，以便了解物品占据多少个网格空间
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::GridFragment);
	//获取图像片段，因为我们要向网格中添加一个小部件，所以需要知道这个特定物品将使用什么纹理或图标
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::IconFragment);
	if (!GridFragment || !ImageFragment) return;

	UInv_SlottedItem* SlottedItem = CreateSlottedItem(Item, bStackable, StackAmount, GridFragment, ImageFragment, Index);

	//将槽位物品添加到画布面板上
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);

	
	//需要将这个新创建的小部件存储在一个数组或某种容器中，以便在丢弃物品、消耗物品、销毁物品或进行任何此类操作时能够移除它
	SlottedItemMap.Add(Index, SlottedItem);
	
}

UInv_SlottedItem* UInv_InventoryGrid::CreateSlottedItem(UInv_InventoryItem* Item, const bool bStackable,
	const int32 StackAmount, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment, const int32 Index)
{
	UInv_SlottedItem* SlottedItems = CreateWidget<UInv_SlottedItem>(GetOwningPlayer(), SlottedItemsClass);
	SlottedItems->SetInventoryItem(Item);
	
	SetSlottedItemImage(SlottedItems, GridFragment, ImageFragment);

	//创建一个控件添加到网格中
	SlottedItems->SetGridIndex(Index);

	SlottedItems->SetIsStackable(bStackable);
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItems->UpdateStackCount(StackUpdateAmount);

	return SlottedItems;
}

void UInv_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment,
	UInv_SlottedItem* SlottedItem) const
{
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));
	const FVector2D DrawPos = UInv_WidgetUtils::GetPositionFromIndex(Index, Columns) * TileSize;
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D(GridFragment->GetGridPadding());

	CanvasSlot->SetPosition(DrawPosWithPadding);
	
	
}

void UInv_InventoryGrid::UpdateGridSlots(UInv_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount)
{
	check(GridSlots.IsValidIndex(Index));

	if (bStackableItem) //左上角的索引 左上角的索引负责记录堆叠数量
	{
		GridSlots[Index]->SetStackCount(StackAmount);
	}

	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(NewItem, FragmentTags::GridFragment);
	if (!GridFragment) return;

	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1);

	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetInventoryItem(NewItem);
		GridSlot->SetUpperLeftIndex(Index);
		GridSlot->SetOccupiedTexture();
		GridSlot->SetAvailable(false);
	});
	
	
}

FVector2D UInv_InventoryGrid::GetDrawSize(const FInv_GridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	return GridFragment->GetGridSize() * IconTileWidth;
}

void UInv_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns); //Reserve : 保留内存，使数组至少可以包含Number元素。

	for (int32 j = 0; j < Rows; j++)
	{
		for (int32 i = 0; i < Columns; i++)
		{
			UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition(i, j);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);

			GridCPS->SetSize(FVector2D(TileSize));
			GridCPS->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}

void UInv_InventoryGrid::SetSlottedItemImage(UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment,
	const FInv_ImageFragment* ImageFragment)
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
}
