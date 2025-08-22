// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Types/Inv_GridTypes.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Inv_ItemManifest.generated.h"


/**
 * 创建库存物品所需的所有信息的一个清单（Manifest）
 */
USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemManifest
{
	GENERATED_BODY()

	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	/**
	 * 创建物品
	 * 传入一个UObject类型的对象，避免在函数内部生成，在这个UObject上附加item component组件
	 * @param NewOuter 需要成为道具的Object对象
	 * @return 
	 */
	UInv_InventoryItem* Manifest(UObject* NewOuter);

	/**
	 * 获取物品的类型Tag
	 * @return 
	 */
	FGameplayTag GetItemTag() const { return ItemTypeTag; }

private:

	//道具类型
	UPROPERTY(EditAnywhere, Category="属性设置")
	EInv_ItemCategory ItemCategory{EInv_ItemCategory::None};

	//物品的类型Tag
	UPROPERTY(editAnywhere, Category="属性设置", meta=(Categories="GameItems")) //添加一个 UPROPERTY(meta = (Categories = "GameItems")) 来筛选编辑器下拉菜单中的标签，使其仅显示以 GameItems 开头的标签
	FGameplayTag ItemTypeTag;

	
	
};
