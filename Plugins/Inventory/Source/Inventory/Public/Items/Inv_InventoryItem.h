// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Manifest/Inv_ItemManifest.h"
#include "Inv_InventoryItem.generated.h"

struct FInstancedStruct;
/**
 * 作为道具的父类
 * 每一个道具分类，都可以从这里进行继承
 */
UCLASS()
class INVENTORY_API UInv_InventoryItem : public UObject
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetItemManifest(const FInv_ItemManifest& Manifest);

protected:
	
private:

	UPROPERTY(VisibleAnywhere, meta=(BaseStruct = "/Script/Inventory.Inv_ItemManifest"), Replicated)//如果我们想通过指定基础结构来限制它，就必须使用元说明符来实现 这里不需要带F
	FInstancedStruct ItemManifest;
	
};
