// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/FastArray/Inv_FastArray.h"


TArray<UInv_InventoryItem*> FInv_InventoryFastArray::GetAllItems() const
{
}

void FInv_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
}

void FInv_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

UInv_InventoryItem* FInv_InventoryFastArray::AddItem(UInv_InventoryComponent* InItemComponent)
{
}

UInv_InventoryItem* FInv_InventoryFastArray::AddItem(UInv_InventoryItem* InItem)
{
}

void FInv_InventoryFastArray::RemoveItem(UInv_InventoryItem* InItem)
{
}
