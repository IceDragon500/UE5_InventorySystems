// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"


TArray<UInv_InventoryItem*> FInv_InventoryFastArray::GetAllItems() const
{
	TArray<UInv_InventoryItem*> Results;
	Results.Reserve(Entries.Num());
	for (const FInv_InventoryEntry& Entry : Entries)
	{
		if (!IsValid(Entry.Item)) continue;
		Results.Add(Entry.Item);
	}

	return Results;
}

void FInv_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	//移除物品之前，我们实际上可以在这里做任何想做的事情
	//如果我们希望被移除的物品能够通过委托广播，以便在其他地方响应
	//广播一个委托，通知所有监听者我们已经从数组中移除了一个物品
	//希望从我的库存组件中广播这个委托

	UInv_InventoryComponent* InventoryComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;

	for (int32 Index : RemovedIndices)
	{
		InventoryComponent->OnItemRemoved.Broadcast(Entries[Index].Item);
	}
	
}


void FInv_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	UInv_InventoryComponent* InventoryComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;

	for (int32 Index : AddedIndices)
	{
		InventoryComponent->OnItemAdded.Broadcast(Entries[Index].Item);
	}
}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_ItemComponent* InItemComponent)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	UInv_InventoryComponent* inventoryComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(inventoryComponent)) return nullptr;

	FInv_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = InItemComponent->GetItemManifest().Manifest(OwningActor);

	inventoryComponent->AddRepSubObj(NewEntry.Item);
	MarkItemDirty(NewEntry);

	return NewEntry.Item;
}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_InventoryItem* InItem)
{
	check(OwnerComponent);
	AActor* OwnerActor = OwnerComponent->GetOwner();
	check(OwnerActor->HasAuthority());

	FInv_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = InItem;

	MarkItemDirty(NewEntry);
	return InItem;
	
}

void FInv_InventoryFastArray::RemoveItem(UInv_InventoryItem* InItem)
{
	for (auto EntryItem = Entries.CreateIterator(); EntryItem; ++EntryItem)
	{
		FInv_InventoryEntry& Entry = *EntryItem;
		if (Entry.Item == InItem)
		{
			EntryItem.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
