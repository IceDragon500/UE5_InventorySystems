// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "Inventory.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Components/Inv_ItemComponent.h"


void UInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(Switcher != nullptr);
	check(Grid_Equippable != nullptr);
	check(Grid_Consumable != nullptr);
	check(Grid_Craftable != nullptr);

	Button_Equippable->OnClicked.AddDynamic(this, &ThisClass::UInv_SpatialInventory::ShowEquippable);
	Button_Consumable->OnClicked.AddDynamic(this, &ThisClass::UInv_SpatialInventory::ShowConsumable);
	Button_Craftable->OnClicked.AddDynamic(this, &ThisClass::UInv_SpatialInventory::ShowCraftable);

	Grid_Equippable->SetOwningCanvas(CanvasPanel);
	Grid_Consumable->SetOwningCanvas(CanvasPanel);
	Grid_Craftable->SetOwningCanvas(CanvasPanel);

	ShowEquippable();
}

FReply UInv_SpatialInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ActiveGrid->DropItem();
	return FReply::Handled(); //112讲 改成这样
	
}

FInv_SlotAvailabilityResult UInv_SpatialInventory::HasRoomForItem(UInv_ItemComponent* ItemComponent) const
{
	//获取到拾取的道具是什么类型的
	//然后通过类型找到对应的道具栏，看是否有空位
	switch (UInv_InventoryStatics::GetItemCategoryFromItemComp(ItemComponent))
	{
		case EInv_ItemCategory::Equippable:
			return Grid_Equippable->HasRoomForItem(ItemComponent);
		case EInv_ItemCategory::Consumable:
			return Grid_Consumable->HasRoomForItem(ItemComponent);
		case EInv_ItemCategory::Craftable:
			return Grid_Craftable->HasRoomForItem(ItemComponent);
		default:
			UE_LOG(LogInventory, Error, TEXT("ItemComponent没有一个有效的ItemCategory"));
			return FInv_SlotAvailabilityResult();
	}
}

void UInv_SpatialInventory::OnItemHovered(UInv_InventoryItem* Item)
{
	Super::OnItemHovered(Item);
}

void UInv_SpatialInventory::OnItemUnHovered()
{
	Super::OnItemUnHovered();
}

bool UInv_SpatialInventory::HasHoverItem() const
{
	if (Grid_Equippable->HasHoverItem()) return true;
	if (Grid_Consumable->HasHoverItem()) return true;
	if (Grid_Craftable->HasHoverItem()) return true;

	return false;
}

void UInv_SpatialInventory::ShowEquippable()
{
	SetActiveGrid(Grid_Equippable, Button_Equippable);
}

void UInv_SpatialInventory::ShowConsumable()
{
	SetActiveGrid(Grid_Consumable, Button_Consumable);
}

void UInv_SpatialInventory::ShowCraftable()
{
	SetActiveGrid(Grid_Craftable, Button_Craftable);
}

void UInv_SpatialInventory::DisableButton(UButton* Button)
{
	//现将所有按钮设置为有效，再将传入的按钮设置为禁用
	Button_Equippable->SetIsEnabled(true);
	Button_Consumable->SetIsEnabled(true);
	Button_Craftable->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

void UInv_SpatialInventory::SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button)
{
	if (ActiveGrid.IsValid()) ActiveGrid->HideCursor();
	ActiveGrid = Grid;
	if (ActiveGrid.IsValid()) ActiveGrid->ShowCursor();
	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}
