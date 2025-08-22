// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Inv_FastArray.generated.h"

class UInv_ItemComponent;
class UInv_InventoryComponent;
class UInv_InventoryItem;

/**
 * 单个道具的类型
 * A single entry in an inventory
 */
USTRUCT(BlueprintType)
struct FInv_InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	FInv_InventoryEntry() {}

private:
	friend struct FInv_InventoryFastArray;
	friend UInv_InventoryComponent;

	UPROPERTY()
	TObjectPtr<UInv_InventoryItem> Item {nullptr};
	
};

/**
 * List of inventory items
 * 物品清单列表
 */
USTRUCT(BlueprintType)
struct FInv_InventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()
public:
	FInv_InventoryFastArray() : OwnerComponent(nullptr) {}
	FInv_InventoryFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

	TArray<UInv_InventoryItem*> GetAllItems() const;
	
	//-------FFastArraySerializer contract-------
	
	/**
	 * 预复制移除
	 * @param RemovedIndices 
	 * @param FinalSize 
	 */
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	
	/**
	 * 复制后添加
	 * @param AddedIndices 
	 * @param FinalSize 
	 */
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	
	//-------End FFastArraySerializer contract-------

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FInv_InventoryEntry, FInv_InventoryFastArray>(Entries, DeltaParams, *this);
	}

	UInv_InventoryItem* AddEntry(UInv_ItemComponent* InItemComponent);
	UInv_InventoryItem* AddEntry(UInv_InventoryItem* InItem);
	void RemoveItem(UInv_InventoryItem* InItem);

private:

	friend UInv_InventoryComponent;

	//物品的复制列表
	UPROPERTY()
	TArray<FInv_InventoryEntry> Entries;

	//所有者组件
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
	
};


template<>
struct TStructOpsTypeTraits<FInv_InventoryFastArray> : public TStructOpsTypeTraitsBase2<FInv_InventoryFastArray>
{
	enum { WithNetDeltaSerializer = true };
};