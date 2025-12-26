// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"
#include "Inventory.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/ItemDescription/Inv_ItemDescription.h"


void UInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(Switcher != nullptr);
	check(Grid_Equippable != nullptr);
	check(Grid_Consumable != nullptr);
	check(Grid_Craftable != nullptr);

	Button_Equippable->OnClicked.AddDynamic(this, &ThisClass::UInv_SpatialInventory::ShowEquippables);
	Button_Consumable->OnClicked.AddDynamic(this, &ThisClass::UInv_SpatialInventory::ShowConsumable);
	Button_Craftable->OnClicked.AddDynamic(this, &ThisClass::UInv_SpatialInventory::ShowCraftables);

	Grid_Equippable->SetOwningCanvas(CanvasPanel);
	Grid_Consumable->SetOwningCanvas(CanvasPanel);
	Grid_Craftable->SetOwningCanvas(CanvasPanel);

	ShowEquippables();

	WidgetTree->ForEachWidget([this](UWidget* Widget)//131讲
	{
		UInv_EquippedGridSlot* EquippedGridSlot = Cast<UInv_EquippedGridSlot>(Widget);
		if (IsValid(EquippedGridSlot))
		{
			EquippedGridSlots.Add(EquippedGridSlot);
			EquippedGridSlot->EquippedGridSlotClicked.AddDynamic(this, &ThisClass::UInv_SpatialInventory::EquippedGridSlotClicked);
		}
	});
}

void UInv_SpatialInventory::EquippedGridSlotClicked(UInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag)
{
	//检查是否能够装备悬停物品,如果我们通过了这一步，那么我们就知道可以装备悬停物品
	if (!CanEquipHoverItem(EquippedGridSlot, EquipmentTypeTag)) return;

	//创建一个已装备的槽位物品并将其添加到装备网格槽中(这里我们需要调用 EquippedGridSlot->OnItemEquipped() )
	UInv_HoverItem* HoverItem = GetHoverItem();
	const float TileSize = UInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetTileSize();
	UInv_EquippedSlottedItem* EquippedSlottedItem = EquippedGridSlot->OnItemEquipped(
		HoverItem->GetInventoryItem(),
		EquipmentTypeTag,
		TileSize
		);

	if (!EquippedSlottedItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("创建失败"));
		return;
	}
	EquippedSlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &UInv_SpatialInventory::EquippedSlottedItemClicked);

	//清除鼠标上的悬停物品hoverItem
	Grid_Equippable->ClearHoverItem();

	//通知服务器我们已经装备了一件物品，其他客户端会看到外观的变化(如果已经有装备在身上了，还涉及卸下一件物品)

	UInv_InventoryComponent* IC = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(IC));

	IC->Server_EquipSlotClicked(HoverItem->GetInventoryItem(), nullptr);

	if (GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)//如果不是专用服务器，144讲
	{
		IC->OnItemEquipped.Broadcast(HoverItem->GetInventoryItem());
	}

	//
}

void UInv_SpatialInventory::EquippedSlottedItemClicked(UInv_EquippedSlottedItem* SlottedItem)
{
	//如果我们鼠标上是空的，点击有装备的装备栏，应该是如下逻辑：
	//如果存在物品描述的小窗口，我们需要将其移除
	UInv_InventoryStatics::ItemUnHovered(GetOwningPlayer());
	
	if (IsValid(GetHoverItem()) && GetHoverItem()->IsStackable()) return;
	
	//获取要装备的物品和获取要卸下的物品
	//需要获取持有此特定物品的已装备网格槽位
	UInv_InventoryItem* ItemToEquip = IsValid(GetHoverItem()) ? GetHoverItem()->GetInventoryItem() : nullptr;
	UInv_InventoryItem* ItemToUnEquip = SlottedItem->GetInventoryItem();

	UInv_EquippedGridSlot* EquippedGridSlot = FindSlotWithEquippedItem(ItemToUnEquip);
	
	//清空该物品的槽位，清空该物品的已装备网格槽位(将其库存物品设为空值)
	ClearSlotOfItem(EquippedGridSlot);

	//需要从已装备网格槽位中移除已装备的槽位物品(涉及诸如解除点击委托绑定等操作 OnEquippedSlottedItemClicked)
	RemoveEquippedSlottedItem(SlottedItem);
	
	//如果我们正在卸载一个装备，应该将其指定为悬停物品，也就是将装备栏上这个道具转到鼠标指针上
	Grid_Equippable->AssignHoverItem(ItemToUnEquip);


	//如果我们鼠标上有一个装备，点击有装备的装备栏，我们会将这两个装备进行比较，然后进行交换，逻辑如下
	//为悬停物品中持有的物品创建一个新的已装备槽位物品，因此我们需要知道要装备的物品是什么，要卸下的物品又是什么

	//最后，在流程结束时，我们应该广播装备物品和卸下物品的委托事件
}

FReply UInv_SpatialInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ActiveGrid->DropItem();
	return FReply::Handled(); //112讲 改成这样
	
}

void UInv_SpatialInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(ItemDescription)) return;

	SetItemDescriptionSizeAndPosition(ItemDescription, CanvasPanel);

	//const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	//GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red,FString::Printf(TEXT("鼠标坐标 x %f, 鼠标坐标 y %f"), MousePosition.X, MousePosition.Y));
	
}

void UInv_SpatialInventory::SetItemDescriptionSizeAndPosition(UInv_ItemDescription* Description, UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
	if (!IsValid(ItemDescriptionCPS)) return;

	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	ItemDescriptionCPS->SetSize(ItemDescriptionSize);

	FVector2D ClampedPosition = UInv_WidgetUtils::GetClampedWidgetPosition(
		UInv_WidgetUtils::GetWidgetSize(Canvas),
		ItemDescriptionSize,
		UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));

	ItemDescriptionCPS->SetPosition(ClampedPosition);
}

bool UInv_SpatialInventory::CanEquipHoverItem(UInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag) const
{
	if (!IsValid(EquippedGridSlot) || EquippedGridSlot->GetInventoryItem().IsValid()) return false;

	UInv_HoverItem* HoverItem = GetHoverItem();
	if (!IsValid(HoverItem)) return false;

	UInv_InventoryItem* HeloItem = HoverItem->GetInventoryItem();

	bool bIsCanEquip = HasHoverItem();
	bool bIsCanEquip1 = IsValid(HeloItem);
	//bool bIsCanEquip2 = !HoverItem->IsStackable();
	bool bIsCanEquip3 = HeloItem->GetItemManifest().GetItemCategory() == EInv_ItemCategory::Equippable;
	bool bIsCanEquip4 = HeloItem->GetItemManifest().GetItemTypeTag().MatchesTag(EquipmentTypeTag);

	return bIsCanEquip && bIsCanEquip1 && bIsCanEquip3 && bIsCanEquip4 ;
	
}

UInv_EquippedGridSlot* UInv_SpatialInventory::FindSlotWithEquippedItem(UInv_InventoryItem* EquippedItem) const
{
	auto* FoundEquippedGridSlot = EquippedGridSlots.FindByPredicate([EquippedItem](const UInv_EquippedGridSlot* GridSlot)
	{
		return GridSlot->GetInventoryItem() == EquippedItem;
	});
	return FoundEquippedGridSlot ? *FoundEquippedGridSlot : nullptr;
}

void UInv_SpatialInventory::ClearSlotOfItem(UInv_EquippedGridSlot* EquippedGridSlot)
{
	if (IsValid(EquippedGridSlot))
	{
		EquippedGridSlot->SetEquippedSlottedItem(nullptr);
		EquippedGridSlot->SetInventoryItem(nullptr);
	}
}

void UInv_SpatialInventory::RemoveEquippedSlottedItem(UInv_EquippedSlottedItem* EquippedSlottedItem)
{
	if (!IsValid(EquippedSlottedItem)) return;

	if (EquippedSlottedItem->OnEquippedSlottedItemClicked.IsAlreadyBound(this, &ThisClass::EquippedSlottedItemClicked))
	{
		EquippedSlottedItem->OnEquippedSlottedItemClicked.RemoveDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	}
	//从父级移除已装备的槽位物品，即从其所属容器中移除
	EquippedSlottedItem->RemoveFromParent();
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
	const auto& Manifest = Item->GetItemManifest();
	UInv_ItemDescription* DescriptionWidget = GetItemDescription();
	DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);

	
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);

	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda([this, &Manifest, DescriptionWidget]()
	{
		//需要在此处将清单数据整合到物品描述控件内部
		Manifest.AssimilateInventoryFragments(DescriptionWidget);
		
		GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
		
	});

	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimer, DescriptionTimerDelegate, DescriptionTimerDelta, false);
}

void UInv_SpatialInventory::OnItemUnHovered()
{
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
}

bool UInv_SpatialInventory::HasHoverItem() const
{
	if (Grid_Equippable->HasHoverItem()) return true;
	if (Grid_Consumable->HasHoverItem()) return true;
	if (Grid_Craftable->HasHoverItem()) return true;

	return false;
}

UInv_HoverItem* UInv_SpatialInventory::GetHoverItem() const
{
	if (!ActiveGrid.IsValid()) return nullptr;

	return ActiveGrid->GetHoverItem();
}

float UInv_SpatialInventory::GetTileSize() const
{
	return Grid_Equippable->GetTileSize();
}

UInv_ItemDescription* UInv_SpatialInventory::GetItemDescription()
{
	if (!IsValid(ItemDescription))
	{
		ItemDescription = CreateWidget<UInv_ItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
		CanvasPanel->AddChild(ItemDescription);
	}
	return ItemDescription;
}

void UInv_SpatialInventory::ShowEquippables()
{
	SetActiveGrid(Grid_Equippable, Button_Equippable);
}

void UInv_SpatialInventory::ShowConsumable()
{
	SetActiveGrid(Grid_Consumable, Button_Consumable);
}

void UInv_SpatialInventory::ShowCraftables()
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