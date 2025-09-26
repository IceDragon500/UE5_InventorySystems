// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Types/Inv_GridTypes.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Inv_ItemManifest.generated.h"


struct FInv_ItemFragment;
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
	FGameplayTag GetItemTypeTag() const { return ItemTypeTag; }

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const;

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	const T* GetFragmentOfType() const;

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	T* GetFragmentOfTypeMutable();

private:

	//用来保存道具的各种“词条”Fragment
	UPROPERTY(EditAnywhere, Category="属性设置", meta=(ExcludeBaseStruct="GameItems"))  //这是一个实例化的结构体，我们可以使用一个名为 ExcludeBaseStruct 的元数据说明符
	TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;

	//ExcludeBaseStruct的作用
	//在细节面板中，我们将能够手动向其添加物品片段，但排除基础结构会阻止我们添加基础结构
	//物品片段在蓝图中只能将物品片段的子项添加到此片段数组中
	
	//道具类型
	UPROPERTY(EditAnywhere, Category="属性设置")
	EInv_ItemCategory ItemCategory{EInv_ItemCategory::None};

	//物品的类型Tag
	UPROPERTY(editAnywhere, Category="属性设置", meta=(Categories="GameItems")) //添加一个 UPROPERTY(meta = (Categories = "GameItems")) 来筛选编辑器下拉菜单中的标签，使其仅显示以 GameItems 开头的标签
	FGameplayTag ItemTypeTag {FGameplayTag()};
	
};


template<typename T> requires std::derived_from<T, FInv_ItemFragment> //requires : 现在这个函数要求 T 必须基于 item fragment 34讲
const T* FInv_ItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag)) continue;
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FInv_ItemFragment>
T* FInv_ItemManifest::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}
