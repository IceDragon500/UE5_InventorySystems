// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_ItemFragment.generated.h"

/**
 * 物品片段
 * 基础结构体
 * 我们将从这个基础结构体派生出更具体的物品片段
 *
 * 希望每个物品片段都是可识别的
 * 希望每个物品片段都是可识别的
 * 希望将物品片段类型定义为gameplaytag
 */

USTRUCT(BlueprintType)
struct FInv_ItemFragment
{
	GENERATED_BODY()

	FInv_ItemFragment() {}

	//---特殊成员函数---

	FInv_ItemFragment(const FInv_ItemFragment&) = default;
	FInv_ItemFragment& operator=(const FInv_ItemFragment&) = default;
	FInv_ItemFragment(FInv_ItemFragment&&) = default;
	FInv_ItemFragment& operator=(FInv_ItemFragment&&) = default;
	virtual ~FInv_ItemFragment() {}

	
	//---特殊成员函数---
	
	//如果这个结构类型的子类被多态地销毁
	//也就是说，通过父类类型的指针，那么父类的析构函数将被调用，并且所有继承链中的析构函数都会依次执行
	//因此，在处理继承时，将析构函数设为虚函数是很重要的

	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }
	
private:

	UPROPERTY(EditAnywhere, Category="属性设置")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
	
};


USTRUCT(BlueprintType)
struct FInv_GridFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(FIntPoint Size) { GridSize = Size; }

	float GetGridPadding() const { return GridPadding; }
	void SetGridPadding(float Padding) { GridPadding = Padding; }

private:

	//道具占多少个格子
	UPROPERTY(EditAnywhere, Category="属性设置")
	FIntPoint GridSize{ 1, 1 };

	//边距
	UPROPERTY(EditAnywhere, Category="属性设置")
	float GridPadding{ 0.f };
	
};

