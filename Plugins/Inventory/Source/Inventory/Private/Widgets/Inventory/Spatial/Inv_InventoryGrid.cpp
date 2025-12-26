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
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Widgets/ItemPopUp/Inv_ItemPopUp.h"
#include "InputCoreTypes.h"
#include "Inventory.h"


void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
	InventoryComponent->OnStackChange.AddDynamic(this, &ThisClass::AddStacks);
	InventoryComponent->OnInventoryMenuToggled.AddDynamic(this, &ThisClass::OnInventoryMenuToggled);
}

void UInv_InventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2D CanvasPos = UInv_WidgetUtils::GetWidgetPosition(CanvasPanel);
	const FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	if (CursorExitedCanvas(CanvasPos, UInv_WidgetUtils::GetWidgetSize(CanvasPanel), MousePos))
	{
		return;
	}

	UpdateTileParameters(CanvasPos, MousePos);
}

void UInv_InventoryGrid::UpdateTileParameters(const FVector2D CanvasPosition, const FVector2D MousePosition)
{
	//如果鼠标没有在道具栏的对应的Canva控件上，我们就直接返回
	if (!bMouseWithinCanvas) return;
	
	// Calculate the tile quadrant, tile index, and coordinates

	const FIntPoint HoveredTileCoordinates = CalculateHoveredCoordinates(CanvasPosition, MousePosition);

	LastTileParameters = TileParameters;
	TileParameters.TileCoordinats = HoveredTileCoordinates;
	TileParameters.TileIndex = UInv_WidgetUtils::GetIndexFromPosition(HoveredTileCoordinates, Columns);
	TileParameters.TileQuadrant = CalculateTileQuadrant(CanvasPosition, MousePosition);
	
	// Handle highlight/unhighlight of the grid slots
	OnTileParametersUpdate(TileParameters);
	
}

void UInv_InventoryGrid::OnTileParametersUpdate(const FInv_TileParameters& Parameters)
{
	if (!IsValid(HoverItem))  return;

	//需要一种简单的方法来存储下面这些信息，以便在需要时随时获取
	//获取悬停物品尺寸
	const FIntPoint Dimensions = HoverItem->GetGridDimensions();
	
	//需要计算高亮显示的起始坐标
	const FIntPoint StartCoordinates = CalculateStartingCoordinate(Parameters.TileCoordinats, Dimensions, Parameters.TileQuadrant);
	ItemDropIndex = UInv_WidgetUtils::GetIndexFromPosition(StartCoordinates, Columns);
	
	//需要检查悬停位置
	CurrentQueryResult = CheckHoverPosition(StartCoordinates, Dimensions);

	if (CurrentQueryResult.bHasSpace)
	{
		HighlightSlots(ItemDropIndex, Dimensions);
		return;
	}

	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);

	if (CurrentQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		//该位置存在可交换或添加的单个物品
		const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(CurrentQueryResult.ValidItem.Get(), FragmentTags::GridFragment);
		if (!GridFragment) return;

		ChangeHoverType(CurrentQueryResult.UpperLeftIndex, GridFragment->GetGridSize(), EInv_GridSlotState::GrayedOut);
	}
}

FInv_SpaceQueryResult UInv_InventoryGrid::CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions)
{
	FInv_SpaceQueryResult Result;
	//需要检查悬停位置
	//在网格边界内吗？
	if (!IsInGridBounds(UInv_WidgetUtils::GetIndexFromPosition(Position, Columns), Dimensions)) return Result;

	Result.bHasSpace = true;
	
	//如果有多个索引被同一物品占据,我们需要检查它们是否都拥有相同的左上角索引

	TSet<int32> OccupiedUpperLeftIndices;
	
	UInv_InventoryStatics::ForEach2D(GridSlots,
		UInv_WidgetUtils::GetIndexFromPosition(Position, Columns),
		Dimensions,
		Columns,
		[&](const UInv_GridSlot* GridSlot)
		{
			if (GridSlot->GetInventoryItem().IsValid())
			{
				OccupiedUpperLeftIndices.Add(GridSlot->GetUpperLeftIndex());
				Result.bHasSpace = false;
			}
		});
	
	//是否有多个物品，如果途中有多个物品，我们就不能在那里放置物品，如果只有一个物品，那我们就可以将鼠标上这个物品与其交换
	if (OccupiedUpperLeftIndices.Num() == 1)//该位置为单一物品 -- 这适用于交换或合并操作
	{
		const int32 Index = *OccupiedUpperLeftIndices.CreateConstIterator();
		Result.ValidItem = GridSlots[Index]->GetInventoryItem();
		Result.UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	}

	return Result;
}

bool UInv_InventoryGrid::CursorExitedCanvas(const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& Location)
{
	bLastMouseWithinCanvas = bMouseWithinCanvas;
	bMouseWithinCanvas = UInv_WidgetUtils::IsWithinBounds(BoundaryPos, BoundarySize, Location);

	if (!bMouseWithinCanvas && bLastMouseWithinCanvas)
	{
		UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
		return true;
	}

	return false;
}

void UInv_InventoryGrid::HighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	if (!bMouseWithinCanvas) return;

	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);

	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetOccupiedTexture();
	});

	LastHighlightedDimensions = Dimensions;
	LastHighlightedIndex = Index;
}

void UInv_InventoryGrid::UnHighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UInv_GridSlot* GridSlot)
	{
		if (GridSlot->IsAvailable())
		{
			GridSlot->SetUnoccupiedTexture();
		}
		else
		{
			GridSlot->SetOccupiedTexture();
		}
	});
}

void UInv_InventoryGrid::ChangeHoverType(const int32 Index, const FIntPoint& Dimensions, EInv_GridSlotState GridSlotState)
{
	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);

	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [State = GridSlotState](UInv_GridSlot* GridSlot)
	{
		switch (State)
		{
			case EInv_GridSlotState::GrayedOut:
				GridSlot->SetGrayedOutTexture();
				break;
			case EInv_GridSlotState::Occupied:
				GridSlot->SetOccupiedTexture();
				break;
			case EInv_GridSlotState::Selected:
				GridSlot->SetSelectedTexture();
				break;
			case EInv_GridSlotState::Unoccupied:
				GridSlot->SetUnoccupiedTexture();
				break;
		}
	});

	LastHighlightedIndex = Index;
	LastHighlightedDimensions = Dimensions;
}

FIntPoint UInv_InventoryGrid::CalculateStartingCoordinate(const FIntPoint& Coordinate, const FIntPoint& Dimensions,
														  EInv_TileQuadrant Quadrant) const
{
	const int32 HasEvenWidth = Dimensions.X % 2 == 0 ? 1 : 0;
	const int32 HasEvenHeight = Dimensions.Y % 2 == 0 ? 1 : 0;

	FIntPoint StartingCoord;
	switch (Quadrant)
	{
	case EInv_TileQuadrant::TopLeft:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X);
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
		break;
	case EInv_TileQuadrant::TopRight:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
		break;
	case EInv_TileQuadrant::BottomLeft:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X);
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
		break;
	case EInv_TileQuadrant::BottomRight:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
		break;
	default:
		UE_LOG(LogInventory, Error, TEXT("Invalid Quadrant."));
		return  FIntPoint(-1, -1);
	}

	return StartingCoord;
}

FIntPoint UInv_InventoryGrid::CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const
{
	return FIntPoint{
		static_cast<int32>(FMath::FloorToInt((MousePosition.X - CanvasPosition.X) / TileSize)),
		static_cast<int32>(FMath::FloorToInt((MousePosition.Y - CanvasPosition.Y) / TileSize))
	};

	/**  FMath::FloorToInt
	* 这个表达式的作用是计算鼠标在网格中的列索引（X坐标方向上的格子序号）。
		以你提供的例子来说：
		MousePosition.X = 660（鼠标X坐标）
		CanvasPosition.X = 500（画布面板X坐标）
		TileSize = 100（每个格子的大小）
		计算过程：
		MousePosition.X - CanvasPosition.X = 660 - 500 = 160
		160 / TileSize = 160 / 100 = 1.6
		FMath::FloorToInt(1.6) = 1（向下取整）
		static_cast<int32>(1) = 1
		所以结果确实是 1，这意味着鼠标位于从画布左边缘开始的第2个格子（索引从0开始）
	 */
}

EInv_TileQuadrant UInv_InventoryGrid::CalculateTileQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const
{
	//计算当前图块内的相对位置

	const float TileLocalX = FMath::Fmod(MousePosition.X - CanvasPosition.X, TileSize);//在我们所处的方格中，我们距离左侧有多远
	const float TileLocalY = FMath::Fmod(MousePosition.Y - CanvasPosition.Y, TileSize);

	//确定鼠标所在的象限

	const bool bIsTop = TileLocalY < TileSize / 2; //如果 y 位于上半部分则为顶部
	const bool bIsLeft = TileLocalX < TileSize / 2;//若 x 不在上半区而在左半区，我们就判定为左侧

	EInv_TileQuadrant HoveredTileQuadrant{EInv_TileQuadrant::None};
	if (bIsTop && bIsLeft) HoveredTileQuadrant = EInv_TileQuadrant::TopLeft;
	else if (bIsTop && !bIsLeft) HoveredTileQuadrant = EInv_TileQuadrant::TopRight;
	else if (!bIsTop && bIsLeft) HoveredTileQuadrant = EInv_TileQuadrant::BottomLeft;
	else if (!bIsTop && !bIsLeft) HoveredTileQuadrant = EInv_TileQuadrant::BottomRight;

	return HoveredTileQuadrant;
	
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_InventoryItem* Item, const int32 StackAmountOverride)
{
	return HasRoomForItem(Item->GetItemManifest(), StackAmountOverride);
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& ItemManifest, const int32 StackAmountOverride)
{
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
	int32 AmountToFill = StackableFragment ? StackableFragment->GetStackCount() : 1;
	if (StackAmountOverride != -1 && Result.bStackable)
	{
		AmountToFill = StackAmountOverride;
	}


	TSet<int32> CheckedIndices;

	//对于每个网格槽位，我们都需要执行一些操作 For each Grid Slot:
	for (const auto& GridSlot : GridSlots)
	{
		//如果没有更多需要填充的，就提前跳出，或者简单地说提前退出循环
		if (AmountToFill == 0) break;

		//这个索引是否已被占用
		if (IsIndexClaimed(CheckedIndices, GridSlot->GetTileIndex())) continue;

		//要判断物品是否在网格边界内
		if (!IsInGridBounds(GridSlot->GetTileIndex(), GetItemDimensions(ItemManifest))) continue;

		//如果这是一个有效的槽位，物品能否放得下,是在检查那些网格尺寸，即物品占用了多少个方格 它是否超出了网格的边界
		TSet<int32> TentativelyClaimed; //可能被占用的索引
		if (!HasRoomAtIndex(GridSlot, GetItemDimensions(ItemManifest), CheckedIndices, TentativelyClaimed,
		                    ItemManifest.GetItemTypeTag(), MaxStackSize))
		{
			continue;
		}

		//要填充多少
		const int32 AmountToFillInSlot = DetermineFillAmountForSlot(Result.bStackable, MaxStackSize, AmountToFill, GridSlot);
		if (AmountToFillInSlot == 0) continue;

		CheckedIndices.Append(TentativelyClaimed);
		
		//更新数量 剩余待填充
		Result.TotalRoomToFill += AmountToFillInSlot;
		Result.SlotAvailabilities.Emplace(
			FInv_SlotAvailability{
				HasValidItem(GridSlot) ? GridSlot->GetUpperLeftIndex() : GridSlot->GetTileIndex(),
				Result.bStackable ? AmountToFillInSlot : 0,
				HasValidItem(GridSlot) 
			}
			);
		
		AmountToFill -= AmountToFillInSlot;

		//当我们遍历完每个槽位后，剩余的数量是多少
		Result.Remainder = AmountToFill;
		
		if (AmountToFill == 0) return Result;
	}
	
	return Result;
}

bool UInv_InventoryGrid::HasRoomAtIndex(const UInv_GridSlot* GridSlot, const FIntPoint& Dimensions,
										const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed,
										const FGameplayTag& ItemType, const int32 MaxStackSize)
{
	//这个索引位置有空位吗 是否有其他物品挡在路上

	bool bHasRoomAtIndex = true;

	UInv_InventoryStatics::ForEach2D(GridSlots, GridSlot->GetTileIndex(), Dimensions, Columns,
									 [&](const UInv_GridSlot* SubGridSlot)
									 {
										 if (CheckSlotConstraints(GridSlot, SubGridSlot, CheckedIndices,
																  OutTentativelyClaimed, ItemType, MaxStackSize))
										 {
											 OutTentativelyClaimed.Add(SubGridSlot->GetTileIndex());
										 }
										 else
										 {
											 bHasRoomAtIndex = false;
										 }
									 });


	return bHasRoomAtIndex;
}

bool UInv_InventoryGrid::CheckSlotConstraints(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot,
											  const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed,
											  const FGameplayTag& ItemType, const int32 MaxStackSize) const
{
	//检查其他任何重要条件 ForEach2D 遍历一个二维范围内的方格(例如一个披风占据了2x3 6个格子，我们需要再进行一个循环来遍历这6个格子是否可用)

	//索引是否已被声明
	if (IsIndexClaimed(CheckedIndices, SubGridSlot->GetTileIndex())) return false;

	//拥有有效物品
	if (!HasValidItem(SubGridSlot))
	{
		OutTentativelyClaimed.Add(SubGridSlot->GetTileIndex());
		return true;
	}

	//这个网格槽位是否是左上角的槽位
	if (!IsUpperLeftSlot(GridSlot, SubGridSlot)) return false;

	//如果是的话，这是可堆叠的物品吗
	const UInv_InventoryItem* SubItem = SubGridSlot->GetInventoryItem().Get();
	if (!SubItem->IsStackable()) return false;

	//如果存在有效物品，我们可以问这个物品是否与我们试图添加的物品类型相同
	if (!DoesItemTypeMatch(SubItem, ItemType)) return false;

	//如果可堆叠的槽位已经达到最大尺寸，或者我应该说已经达到最大堆叠尺寸，那么它是否处于最大容量
	if (GridSlot->GetStackCount() >= MaxStackSize) return false;


	return true;
}

FIntPoint UInv_InventoryGrid::GetItemDimensions(const FInv_ItemManifest& ItemManifest) const
{
	const FInv_GridFragment* GridFragment = ItemManifest.GetFragmentOfType<FInv_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
}

bool UInv_InventoryGrid::HasValidItem(const UInv_GridSlot* GridSlot) const
{
	return GridSlot->GetInventoryItem().IsValid();
}

bool UInv_InventoryGrid::IsUpperLeftSlot(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot) const
{
	return SubGridSlot->GetUpperLeftIndex() == GridSlot->GetUpperLeftIndex();
}

bool UInv_InventoryGrid::DoesItemTypeMatch(const UInv_InventoryItem* SubItem, const FGameplayTag& ItemType) const
{
	return SubItem->GetItemManifest().GetItemTypeTag().MatchesTagExact(ItemType);
}

bool UInv_InventoryGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const
{
	if (StartIndex <0 || StartIndex >= GridSlots.Num()) return false;
	const int32 EndColum = (StartIndex % Columns) + ItemDimensions.X;
	const int32 EndRow = (StartIndex / Columns) + ItemDimensions.Y;

	return EndColum <= Columns && EndRow <= Rows;
}

int32 UInv_InventoryGrid::DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize,
	const int32 AmountToFill, const UInv_GridSlot* GirdSlot) const
{
	//计算槽位中的剩余空间
	const int32 RoomInSlot = MaxStackSize - GetStackAmount(GirdSlot);
	
	//如果是可堆叠的，我们需要最小值在填充数量AmountToFill和槽位剩余空间 RooInSlot之间
	return bStackable ? FMath::Min(AmountToFill, RoomInSlot) : 1;
}

int32 UInv_InventoryGrid::GetStackAmount(const UInv_GridSlot* GridSlot) const
{
	int32 CurrentSlotStackCount = GridSlot->GetStackCount();

	//如果我们所在的槽位不存放堆叠数量,就必须获取实际的堆叠数量
	//如果我们所在的槽位不存放堆叠数量，那就意味着我们并非左上角
	if (const int32 UpperLeftIndex = GridSlot->GetUpperLeftIndex() ; UpperLeftIndex!= INDEX_NONE)
	{
		UInv_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
		CurrentSlotStackCount = UpperLeftGridSlot->GetStackCount();
	}
	
	return CurrentSlotStackCount;
}

bool UInv_InventoryGrid::IsRightClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UInv_InventoryGrid::IsLeftClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

void UInv_InventoryGrid::PickUp(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	//分配HoverItem
	AssignHoverItem(ClickedInventoryItem, GridIndex, GridIndex);
	//从网格中移除被点击的项
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
}

void UInv_InventoryGrid::AssignHoverItem(UInv_InventoryItem* InventoryItem, const int32 GridIndex,const int32 PreviousGridIndex)
{
	AssignHoverItem(InventoryItem);

	HoverItem->SetPreviousGridIndex(PreviousGridIndex);
	HoverItem->UpdateStackCount(InventoryItem->IsStackable() ? GridSlots[GridIndex]->GetStackCount() : 0);
}

void UInv_InventoryGrid::OnHide()
{
	PutHoverItemBack();
}

void UInv_InventoryGrid::RemoveItemFromGrid(UInv_InventoryItem* InventoryItem, const int32 GridIndex)
{
	//获取物品的网格片段
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	if (!GridFragment) return;

	//遍历物品的网格空间
	UInv_InventoryStatics::ForEach2D(GridSlots, GridIndex, GridFragment->GetGridSize(), Columns, [&](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetInventoryItem(nullptr);
		GridSlot->SetUpperLeftIndex(INDEX_NONE);
		GridSlot->SetUnoccupiedTexture();
		GridSlot->SetAvailable(true);
		GridSlot->SetStackCount(0);
	});

	//彻底移除已放置的物品
	if (SlottedItemMap.Contains(GridIndex))
	{
		TObjectPtr<UInv_SlottedItem> FoundSlottedItem;
		SlottedItemMap.RemoveAndCopyValue(GridIndex, FoundSlottedItem);
		FoundSlottedItem->RemoveFromParent();
	}
}

void UInv_InventoryGrid::AssignHoverItem(UInv_InventoryItem* InventoryItem)
{
	if (!IsValid(HoverItem))
	{
		HoverItem = CreateWidget<UInv_HoverItem>(GetOwningPlayer(), HoverItemClass);
		//HoverItem->SetInventoryItem(InventoryItem);
	}

	//设定获取网格片段的功能，以便了解物品占据多少个网格空间
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	//获取图像片段，因为我们要向网格中添加一个小部件，所以需要知道这个特定物品将使用什么纹理或图标
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(InventoryItem, FragmentTags::IconFragment);
	if (!GridFragment || !ImageFragment) return;

	const FVector2D DrawSize = GetDrawSize(GridFragment);
	FSlateBrush IconBrush;
	IconBrush.SetResourceObject(ImageFragment->GetIcon());
	IconBrush.DrawAs = ESlateBrushDrawType::Image;
	IconBrush.ImageSize = DrawSize * UWidgetLayoutLibrary::GetViewportScale(this);//如果视口以不协调的方式缩放，这将进行补偿

	HoverItem->SetImageBrush(IconBrush);
	HoverItem->SetGridDimensions(GridFragment->GetGridSize());
	HoverItem->SetInventoryItem(InventoryItem);
	HoverItem->SetIsStackable(InventoryItem->IsStackable());

	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, HoverItem);
}

void UInv_InventoryGrid::AddStacks(const FInv_SlotAvailabilityResult& Result)
{
	if (!MatchesCategory(Result.Item.Get())) return;

	for (const auto& Availability : Result.SlotAvailabilities)
	{
		if (Availability.bItemAtIndex)
		{
			const auto& GridSlot = GridSlots[Availability.Index];
			const auto& SlottedItem = SlottedItemMap.FindChecked(Availability.Index);
			SlottedItem->UpdateStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
			GridSlot->SetStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
		}
		else
		{
			AddItemAtIndex(Result.Item.Get(), Availability.Index, Result.bStackable, Availability.AmountToFill);
			UpdateGridSlots(Result.Item.Get(), Availability.Index, Result.bStackable, Availability.AmountToFill);
		}
	}
}

void UInv_InventoryGrid::OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	UInv_InventoryStatics::ItemUnHovered(GetOwningPlayer());
	
	check(GridSlots.IsValidIndex(GridIndex));
	UInv_InventoryItem* ClickedInventoryItem = GridSlots[GridIndex]->GetInventoryItem().Get();

	//创建一个悬停物品,并将其画刷设置为该库存物品的图标
	//同时也要将该库存物品从网格中移除

	if (!IsValid(HoverItem) && IsLeftClick(MouseEvent))
	{
		PickUp(ClickedInventoryItem, GridIndex);
		return;
	}
	
	if (IsRightClick(MouseEvent))
	{
		CreateItemPopUp(GridIndex);
		return;
	}

	//鼠标上的物品和被点击的物品，是否属于同一类型且可堆叠，如果它们是相同类型且可堆叠，那么：
	if (IsSameStackable(ClickedInventoryItem))
	{
		const int32 ClickedStackCount = GridSlots[GridIndex]->GetStackCount();
		const FInv_StackableFragment* StackableFragment = ClickedInventoryItem->GetItemManifest().GetFragmentOfType<FInv_StackableFragment>();
		const int32 MaxStackSize = StackableFragment->GetMaxStackSize();
		const int32 RoomInClickedSlot = MaxStackSize - ClickedStackCount;
		const int32 HoveredStackCount = HoverItem->GetStackCount();
		
		//是否应该交换它们的堆叠数量（例如，背包中一个最大10堆叠的红药瓶和1个红药瓶，鼠标点起1个红药瓶，再点击10个堆叠的红药瓶，只需要交换他们的堆叠数量即可，没有必要做到完全交换，因为是同一个道具）
		if (ShouldSwapStackCounts(RoomInClickedSlot, HoveredStackCount,MaxStackSize))
		{
			SwapStackCounts(ClickedStackCount, HoveredStackCount, GridIndex);
			return;
		}
		
		//是否应该消耗悬停物品的堆叠，将HoverItem的数量加到被点击物品的堆叠数量中（例如，背包中一个9堆叠的红药瓶和1个红药瓶，鼠标点起1个红药瓶，再点击9个堆叠的红药瓶，只需要增加被点击物品的堆叠数量即可，然后清除鼠标上的HoverItem）
		if (ShouldConsumeHoverItemStacks(HoveredStackCount, RoomInClickedSlot))
		{
			ConsumeHoverItemStacks(ClickedStackCount, HoveredStackCount, GridIndex);
			return;
		}
		
		//如果我们的悬停物品超出可填充容量，是否应该填充被点击物品的堆叠数量，而不消耗悬停物品
		if (ShouldFillInStack(RoomInClickedSlot, HoveredStackCount))
		{
			FillInStack(RoomInClickedSlot, HoveredStackCount - RoomInClickedSlot, GridIndex);
			return;
		}
		
		//点击的槽位已经满 什么都不做
		if (RoomInClickedSlot == 0)
		{
			return;
		}
	}

	//确保可以与有效物品进行交换，避免两个小物品放在一起时，如果用一个大图标物品放置，会导致一个小物品被放在了大物品图标里面
	if (CurrentQueryResult.ValidItem.IsValid())
	{
		//如果我们在最后没有提前返回，就应该与悬停物品交换
		SwapWithHoverItem(ClickedInventoryItem, GridIndex);
	}

	
}

void UInv_InventoryGrid::DropItem()
{
	if (!IsValid(HoverItem)) return;
	if (!IsValid(HoverItem->GetInventoryItem())) return;

	//指示服务器实际执行物品丢弃操作，这涉及生成物品的过程、从库存中移除物品等操作
	InventoryComponent->Server_DropItem(HoverItem->GetInventoryItem(), HoverItem->GetStackCount());

	ClearHoverItem();
	ShowCursor();
}

bool UInv_InventoryGrid::HasHoverItem() const
{
	return IsValid(HoverItem);
}

UInv_HoverItem* UInv_InventoryGrid::GetHoverItem() const
{
	return HoverItem;
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
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		AddItemAtIndex(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
		UpdateGridSlots(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
	}
}

void UInv_InventoryGrid::AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bStackable,
                                        const int32 StackAmount)
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

UInv_SlottedItem* UInv_InventoryGrid::CreateSlottedItem(UInv_InventoryItem* Item, const bool bStackable, const int32 StackAmount, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment, const int32 Index)
{
	UInv_SlottedItem* SlottedItem = CreateWidget<UInv_SlottedItem>(GetOwningPlayer(), SlottedItemsClass);
	SlottedItem->SetInventoryItem(Item);

	SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);

	//创建一个控件添加到网格中
	SlottedItem->SetGridIndex(Index);

	SlottedItem->SetIsStackable(bStackable);
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItem->UpdateStackCount(StackUpdateAmount);
	SlottedItem->OnSlottedItemClicked.AddDynamic(this, &ThisClass::OnSlottedItemClicked);

	return SlottedItem;
}

void UInv_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment, UInv_SlottedItem* SlottedItem) const
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

	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetInventoryItem(NewItem);
		GridSlot->SetUpperLeftIndex(Index);
		GridSlot->SetOccupiedTexture();
		GridSlot->SetAvailable(false);
	});
}

bool UInv_InventoryGrid::IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const
{
	return CheckedIndices.Contains(Index);
}

FVector2D UInv_InventoryGrid::GetDrawSize(const FInv_GridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	return GridFragment->GetGridSize() * IconTileWidth;
}

void UInv_InventoryGrid::SetSlottedItemImage(const UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
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

			GridSlot->GridSlotClicked.AddDynamic(this, &ThisClass::OnGridSlotClicked);
			GridSlot->GridSlotHovered.AddDynamic(this, &ThisClass::OnGridSlotHovered);
			GridSlot->GridSlotUnHovered.AddDynamic(this, &ThisClass::OnGridSlotUnHovered);
		}
	}
}

void UInv_InventoryGrid::OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	//如果我们没有悬停物品，就不需要执行任何操作
	if (!IsValid(HoverItem)) return;

	//当我们点击带有悬停物品的网格槽位时，我们希望尝试将该物品放置到目标位置

	//当我们点击网格槽位时，需要确保该物品放置索引是有效的
	if (!GridSlots.IsValidIndex(ItemDropIndex)) return;

	//检查一下当前的查询结果是否拥有有效的物品
	//如果存在有效的物品，则需要将物品和道具栏中的物品进行交换
	if (CurrentQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		OnSlottedItemClicked(CurrentQueryResult.UpperLeftIndex, MouseEvent);
		return;
	}

	//如果没有有效的物品，则说明这里是个空位置，我们需要将物品放置在此索引位置
	auto GridSlot = GridSlots[ItemDropIndex];
	if (!GridSlot->GetInventoryItem().IsValid())
	{
		//TODO : 将物品放置在此索引位置
		PutDownOnIndex(ItemDropIndex);
	}
	
}

void UInv_InventoryGrid::PutDownOnIndex(const int32 Index)
{
	AddItemAtIndex(HoverItem->GetInventoryItem(), Index, HoverItem->IsStackable(), HoverItem->GetStackCount());
	UpdateGridSlots(HoverItem->GetInventoryItem(), Index, HoverItem->IsStackable(), HoverItem->GetStackCount());
	ClearHoverItem();
}

void UInv_InventoryGrid::ClearHoverItem()
{
	if (!IsValid(HoverItem)) return;

	HoverItem->SetInventoryItem(nullptr);
	HoverItem->SetIsStackable(false);
	HoverItem->SetPreviousGridIndex(INDEX_NONE);
	HoverItem->UpdateStackCount(0);
	HoverItem->SetImageBrush(FSlateNoResource());

	HoverItem->RemoveFromParent();
	HoverItem = nullptr;

	//TODO : 显示鼠标光标
	ShowCursor();
	
}

UUserWidget* UInv_InventoryGrid::GetVisibleCursorWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;
	if (!IsValid(VisibleCursorWidget))
	{
		VisibleCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), VisibleCursorWidgetClass);
	}

	return VisibleCursorWidget;
}

UUserWidget* UInv_InventoryGrid::GetHiddenCurosrWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;
	if (!IsValid(HiddenCursorWidget))
	{
		HiddenCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), HiddenCursorWidgetClass);
	}

	return HiddenCursorWidget;
}

bool UInv_InventoryGrid::IsSameStackable(const UInv_InventoryItem* ClickedInventoryItem) const
{
	const bool bIsSameItem = ClickedInventoryItem == HoverItem->GetInventoryItem();
	const bool bIsStackable = ClickedInventoryItem->IsStackable();

	return bIsSameItem && bIsStackable && HoverItem->GetItemType().MatchesTagExact(ClickedInventoryItem->GetItemManifest().GetItemTypeTag());
}

void UInv_InventoryGrid::SwapWithHoverItem(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	if (!IsValid(HoverItem)) return;

	UInv_InventoryItem* TempInventoryItem = HoverItem->GetInventoryItem();
	const int32 TempStackCount = HoverItem->GetStackCount();
	const bool bTempIsStackable = HoverItem->IsStackable();

	//keep the same previous grid index

	AssignHoverItem(ClickedInventoryItem, GridIndex, HoverItem->GetPreviousGridIndex());

	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);

	AddItemAtIndex(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);

	UpdateGridSlots(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);
}

bool UInv_InventoryGrid::ShouldSwapStackCounts(const int32 RoomInClickedSlot, const int32 HoveredStackCount,
	const int32 MaxStackSize) const
{
	return RoomInClickedSlot == 0 && HoveredStackCount < MaxStackSize;
}

void UInv_InventoryGrid::SwapStackCounts(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index)
{
	//先将道具格子中的道具数量设置为鼠标上的道具数量
	UInv_GridSlot* GirdSlot = GridSlots[Index];
	GirdSlot->SetStackCount(HoveredStackCount);

	UInv_SlottedItem* ClickedSlottedItem = SlottedItemMap.FindChecked(Index);
	ClickedSlottedItem->UpdateStackCount(HoveredStackCount);

	HoverItem->UpdateStackCount(ClickedStackCount);
	
}

bool UInv_InventoryGrid::ShouldConsumeHoverItemStacks(const int32 HoveredStackCount,
	const int32 RoomInClickedSlot) const
{
	return RoomInClickedSlot >= HoveredStackCount;
}

void UInv_InventoryGrid::ConsumeHoverItemStacks(const int32 ClickedStackCount, const int32 HoveredStackCount,
	const int32 Index)
{
	const int32 AmountToTransfer = HoveredStackCount;
	const int32 NewClickedStackCount = ClickedStackCount + AmountToTransfer;

	GridSlots[Index]->SetStackCount(NewClickedStackCount);
	SlottedItemMap.FindChecked(Index)->UpdateStackCount(NewClickedStackCount);

	//这里会有点问题
	
	ClearHoverItem();

	ShowCursor();

	const FInv_GridFragment* GridFragment = GridSlots[Index]->GetInventoryItem()->GetItemManifest().GetFragmentOfType<FInv_GridFragment>();
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

	HighlightSlots(Index, Dimensions);
}

bool UInv_InventoryGrid::ShouldFillInStack(const int32 RoomInClickedSlot, const int32 HoveredStackCount)
{
	return RoomInClickedSlot < HoveredStackCount;
}

void UInv_InventoryGrid::FillInStack(const int32 FillAmount, const int32 Remainder, const int32 Index)
{
	UInv_GridSlot* GridSlot = GridSlots[Index];
	const int32 NewStackCount = GridSlot->GetStackCount() + FillAmount;

	GridSlot->SetStackCount(NewStackCount);

	UInv_SlottedItem* ClickedSlottedItem = SlottedItemMap.FindChecked(Index);
	ClickedSlottedItem->UpdateStackCount(NewStackCount);

	HoverItem->UpdateStackCount(Remainder);
}

void UInv_InventoryGrid::CreateItemPopUp(const int32 GridIndex)
{
	UInv_InventoryItem* RightClickedItem = GridSlots[GridIndex]->GetInventoryItem().Get();

	if (!IsValid(RightClickedItem)) return;

	if (IsValid(GridSlots[GridIndex]->GetItemPopUp().Get()))
	{
		//GridSlots[GridIndex]->SetItemPopUp(nullptr);
		return;
	}

	ItemPopUp = CreateWidget<UInv_ItemPopUp>(this, ItemPopUpClass);
	GridSlots[GridIndex]->SetItemPopUp(ItemPopUp);

	OwningCanvasPanel->AddChildToCanvas(ItemPopUp);

	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemPopUp);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	CanvasSlot->SetPosition(MousePosition - ItemPopUpOffset);
	CanvasSlot->SetSize(ItemPopUp->GetBoxSize());

	const int32 SliderMax = GridSlots[GridIndex]->GetStackCount() - 1;

	//当物品可以拆分，显示拆分按钮  绑定OnPopUpMenuSplit
	if (RightClickedItem->IsStackable() && SliderMax > 0)
	{
		ItemPopUp->OnPopUpMenuSplit.BindDynamic(this, &ThisClass::OnPopUpMenuSplit);
		ItemPopUp->SetSliderParams(SliderMax, FMath::Max(1, GridSlots[GridIndex]->GetStackCount() / 2));
	}
	else
	{
		ItemPopUp->CollapseSplitButton();
	}

	//显示扔掉按钮，绑定OnPopUpMenuDrop
	ItemPopUp->OnPopUpMenuDrop.AddDynamic(this, &ThisClass::OnPopUpMenuDrop);//我这里是多播，所以用AddDynamic 和教程不一样

	//显示使用按钮，绑定OnPopUpMenuConsume
	if (RightClickedItem->IsConsumable())
	{
		ItemPopUp->OnPopUpMenuConsume.AddDynamic(this, &ThisClass::OnPopUpMenuConsume);
	}
	else
	{
		ItemPopUp->CollapseConsumeButton();
	}
}

void UInv_InventoryGrid::PutHoverItemBack()
{
	if (!IsValid(HoverItem)) return;

	FInv_SlotAvailabilityResult Result = HasRoomForItem(HoverItem->GetInventoryItem(), HoverItem->GetStackCount());
	Result.Item = HoverItem->GetInventoryItem();

	AddStacks(Result);
	ClearHoverItem();
}

void UInv_InventoryGrid::ShowCursor()
{
	if (!IsValid(GetOwningPlayer())) return;
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetVisibleCursorWidget());
}

void UInv_InventoryGrid::HideCursor()
{
	if (!IsValid(GetOwningPlayer())) return;
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetHiddenCurosrWidget());
}

void UInv_InventoryGrid::SetOwningCanvas(UCanvasPanel* OwningCanvas)
{
	OwningCanvasPanel = OwningCanvas;
}

void UInv_InventoryGrid::OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem)) return;

	UInv_GridSlot* GridSlot = GridSlots[GridIndex];

	if (GridSlot->IsAvailable())
	{
		GridSlot->SetOccupiedTexture();
	}
}

void UInv_InventoryGrid::OnGridSlotUnHovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem)) return;

	UInv_GridSlot* GridSlot = GridSlots[GridIndex];

	if (GridSlot->IsAvailable())
	{
		GridSlot->SetUnoccupiedTexture();
	}
}

void UInv_InventoryGrid::OnPopUpMenuSplit(int32 SplitAmount, int32 Index)
{
	UInv_InventoryItem* RightClickItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickItem)) return;
	if (!RightClickItem->IsStackable()) return;

	const int32 UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	UInv_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
	const int32 StackCount = UpperLeftGridSlot->GetStackCount();
	const int32 NewStackCount = StackCount - SplitAmount;

	UpperLeftGridSlot->SetStackCount(NewStackCount);
	SlottedItemMap.FindChecked(UpperLeftIndex)->UpdateStackCount(NewStackCount);

	AssignHoverItem(RightClickItem, UpperLeftIndex, UpperLeftIndex);
	HoverItem->UpdateStackCount(SplitAmount);
}

void UInv_InventoryGrid::OnPopUpMenuDrop(int32 Index)
{
	UInv_InventoryItem* RightClickItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickItem)) return;

	PickUp(RightClickItem, Index);
	DropItem();
	
}

void UInv_InventoryGrid::OnPopUpMenuConsume(int32 Index)
{
	UInv_InventoryItem* RightClickItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickItem)) return;

	const int32 UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	UInv_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
	const int32 NewStackCount = UpperLeftGridSlot->GetStackCount() - 1;

	UpperLeftGridSlot->SetStackCount(NewStackCount);
	SlottedItemMap.FindChecked(UpperLeftIndex)->UpdateStackCount(NewStackCount);

	//通知服务器我们正在消耗一个物品,这将涉及到我们实际的库存
	InventoryComponent->Server_ConsumeItem(RightClickItem);

	//如果数量归0，意思就是这个格子的道具被消耗完了，也需要删除这个格子上的道具
	if (NewStackCount <= 0)
	{
		RemoveItemFromGrid(RightClickItem, Index);
	}
	
}

void UInv_InventoryGrid::OnInventoryMenuToggled(bool bOpen)
{
	if (!bOpen)
	{
		PutHoverItemBack();
	}
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}
