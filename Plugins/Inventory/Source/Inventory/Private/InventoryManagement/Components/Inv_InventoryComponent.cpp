// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/Inv_InventoryComponent.h"

#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"


UInv_InventoryComponent::UInv_InventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;

}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
	FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

	UInv_InventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemTypeTag());
	Result.Item = FoundItem;

	if (Result.TotalRoomToFill == 0)
	{
		NoRoomInInventory.Broadcast();
		return;
	}

	/**
	 * TODO： Actually add the item to the inventory
	 * 如果是在多人游戏中，我们还需要让客户端告诉服务器添加这个物品，并实际由服务器来执行添加操作，并将结果复制回客户端
	 * 所以 我们需要一个服务器RPC来告知服务器我们需要添加这个物品
	 * 但是实际上 我们需要考虑两种情况
	 * 1、如果物品是可以堆叠的，并且库存中已经有这种物品了应该怎么办，这种情况下，我们只需要增加堆叠数量就可以了
	 * 2、如果物品尚未存在于物品栏中，这时候我们就需要新建一个物品并将其添加到物品栏中
	 */

	if (Result.Item.IsValid() && Result.bStackable)//如果已经有这个道具并且这个道具是可以被堆叠的
	{
		// Result.Item.IsValid() 这意味着库存中已经存在这种类型的物品
		// Result.bStackable 表示物品是可以被堆叠的
		// 如果两个都通过，说明我们在尝试添加已经已经在道具栏里面的可堆叠物品
		// Add stacks to an item that already exists in the inventory. We only want ot update the stack count,
		// not create a new item of this type

		OnStackChange.Broadcast(Result);
		Server_AddStacksToItem(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
		
	}
	else if (Result.TotalRoomToFill > 0) //检查有剩余空间
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
	//创建一个新的库存道具
	UInv_InventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);
	NewItem->SetTotalStackCount(StackCount);

	//NM_ListenServer 作为服务器的客户端
	//NM_Standalone 单机玩家
	// 上面这两中情况 将视为他自己就拥有权限 这种情况下 不会进行数组复制，因为没有客户端需要复制
	
	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		//如果是以上两种情况，添加物品的时候广播这个物品
		//否则我们将让物品复制添加处理广播此物品
		OnItemAdded.Broadcast(NewItem);
	}

	//TODO: 让物品组件销毁其所属的 Actor
	ItemComponent->PickedUp();
}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	//首先获取要添加物品的类型标签
	const FGameplayTag& ItemType = IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemTypeTag() : FGameplayTag::EmptyTag;

	//在库存中查找相同类型的物品
	UInv_InventoryItem* Item = InventoryList.FindFirstItemByType(ItemType);

	//如果找到有效物品，则增加其堆叠计数
	if (!IsValid(Item)) return;
	
	Item->SetTotalStackCount(Item->GetTotalStackCount() + StackCount);

	//TODO: 如果余数为零则销毁该物品
	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FInv_StackableFragment* StackableFragment = ItemComponent->GetItemManifest().GetFragmentOfTypeMutable<FInv_StackableFragment>())//否则的话,更新地面上那个物品拾取物的堆叠数量
	{
		StackableFragment->SetStackCount(Remainder);
	}
	
	
}

void UInv_InventoryComponent::Server_DropItem_Implementation(UInv_InventoryItem* DropItem, int32 DropCount)
{
	const int32 NewStackCount = DropItem->GetTotalStackCount() - DropCount;
	if (NewStackCount <= 0)
	{
		//如果当前被丢弃的数量与当前这个道具堆叠数量相同，则表示需要从InventoryList移除这个道具
		InventoryList.RemoveEntry(DropItem);
	}
	else
	{
		//如果数量小于这个道具的堆叠数量，也就是说这个道具还有剩，则只需要设置一下这个道具堆叠数量
		DropItem->SetTotalStackCount(NewStackCount);
	}

	SpawnDroppedItem(DropItem, DropCount);
}

void UInv_InventoryComponent::SpawnDroppedItem(UInv_InventoryItem* Item, int32 StackCount)
{
	//在角色正前方一个夹角内，随机使用一个旋转度数，获得一个位置 生成道具
	const APawn* OwningPawn = OwningController->GetPawn();
	FVector RotatedForward = OwningPawn->GetActorForwardVector();
	RotatedForward = RotatedForward.RotateAngleAxis(FMath::FRandRange(DropSpawnAngleMin, DropSpawnAngleMax), FVector::UpVector);
	FVector SpawnLocation = OwningPawn->GetActorLocation() + RotatedForward * FMath::FRandRange(DropSpawnDistanceMin, DropSpawnAngleMax);
	SpawnLocation.Z = RelativeSpawnElevation;
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	//接下来我们要让物品清单生成可拾取道具的 Actor
	FInv_ItemManifest& ItemManifest = Item->GetItemManifestMutable();
	if (FInv_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_StackableFragment>())
	{
		StackableFragment->SetStackCount(StackCount);
	}
	ItemManifest.SpawnPickupActor(this, SpawnLocation, SpawnRotation);
}

void UInv_InventoryComponent::Server_ConsumeItem_Implementation(UInv_InventoryItem* Item)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - 1;
	if (NewStackCount <=0 )
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	//获取消耗品片段并调用消耗功能
	if (FInv_ConsumableFragment* ConsumableFragment = Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FInv_ConsumableFragment>())
	{
		ConsumableFragment->OnConsume(OwningController.Get());
	}
}

void UInv_InventoryComponent::Server_EquipSlotClicked_Implementation(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnEquip)
{
	Multicast_EquipSlotClicked(ItemToEquip, ItemToUnEquip);
}

void UInv_InventoryComponent::Multicast_EquipSlotClicked_Implementation(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnEquip)
{
	//装备组件将会监听这些委托
	OnItemEquipped.Broadcast(ItemToEquip);
	OnItemUnEquipped.Broadcast(ItemToUnEquip);
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
	OnInventoryMenuToggled.Broadcast(bInventoryMenuOpen);
}

void UInv_InventoryComponent::AddRepSubObj(UObject* SubObj)
{
	/**
	 * IsUsingRegisteredSubObjectList()
	 * 检查此组件是否被配置为使用“注册子对象列表”的方式进行复制。这取决于组件创建时设置的 bReplicateUsingRegisteredSubObjectList 标志（在你的构造函数中它被设为 true）
	 *
	 * IsReadyForReplication()
	 * 检查此组件是否已经初始化完毕，准备好进行复制。
	 *
	 * AddReplicatedSubObject
	 * 将一个子对象（SubObject）显式地注册到其父组件的复制系统中，以确保该子对象能够随其父组件一起进行网络复制。
	 * 
	 */
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}


void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ConstructInventory();
}

void UInv_InventoryComponent::ConstructInventory()
{
	//获取并验证所有者 (Owner Acquisition and Validation)
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("玩家控制器转换失败"))

	//本地控制检查 (Local Control Check)
	if (!OwningController->IsLocalController()) return;

	//创建用户界面控件 (User Interface Widget Creation)
	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();

	//初始化UI状态 (Initial UI State Setup)
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

	/* 这里是我自己的做法，教程在151讲后半段提到了解决的办法
	 * 
	当鼠标上有道具时，点击关闭窗口，这里需要将鼠标上的道具扔在地上，并且清理掉鼠标上的道具
	UInv_HoverItem* HoverItem = GetInventoryMenu()->GetHoverItem();
	if (!IsValid(HoverItem)) return;
	Server_DropItem(HoverItem->GetInventoryItem(), 1);
	HoverItem->SetInventoryItem(nullptr);
	HoverItem->SetIsStackable(false);
	HoverItem->SetPreviousGridIndex(INDEX_NONE);
	HoverItem->UpdateStackCount(0);
	HoverItem->SetImageBrush(FSlateNoResource());
	HoverItem->RemoveFromParent();
	HoverItem = nullptr;
	*/
}
