// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/Inv_InventoryComponent.h"

#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"


UInv_InventoryComponent::UInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
	FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

	if (Result.TotalRoomToFill == 0)
	{
		NoRoomInInventory.Broadcast();
	}

	/**
	 * TODO： Actually add the item to the inventory
	 * 如果是在多人游戏中，我们还需要让客户端告诉服务器添加这个物品，并实际由服务器来执行添加操作，并将结果复制回客户端
	 * 所以 我们需要一个服务器RPC来告知服务器我们需要添加这个物品
	 * 但是实际上 我们需要考虑两种情况
	 * 1、如果物品是可以堆叠的，并且库存中已经有这种物品了应该怎么办，这种情况下，我们只需要增加堆叠数量就可以了
	 * 2、如果物品尚未存在于物品栏中，这时候我们就需要新建一个物品并将其添加到物品栏中
	 */

	if (Result.Item.IsValid() && Result.bStackable)
	{
		// Result.Item.IsValid() 这意味着库存中已经存在这种类型的物品
		// Result.bStackable 表示物品是可以被堆叠的
		// 如果两个都通过，说明我们在尝试添加已经已经在道具栏里面的可堆叠物品
		// Add stacks to an item that already exists in the inventory. We only want ot update the stack count,
		// not create a new item of this type

		Server_AddStacksToItem(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
		
	}
	else if (Result.TotalRoomToFill > 0)
	{
		//否则我们需要判断是否有剩余空间来存放这个尚未存在的物品
		//这个项目类型不在库存中，创建一个新的并更新所有相关的插槽
		//This item type doesn't in the inventory, Create a new one and update all pertinent slots

		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0);
		//Result.bStackable ? Result.TotalRoomToFill : 0
		//如果是可以堆叠的，这里就是TotalRoomToFill，如果不是，那这里就是0
	}
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount)
{
}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
}


void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ConstructInventory();


	
}

void UInv_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("玩家控制器转换失败"))
	if (!OwningController->IsLocalController()) return;

	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	CloseInventoryMenu();
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;
	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningController.IsValid()) return;
	
	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
	
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;
	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningController.IsValid()) return;
	
	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
}
