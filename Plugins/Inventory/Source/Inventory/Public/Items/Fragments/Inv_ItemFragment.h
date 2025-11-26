// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/Composite/Inv_CompositeBase.h"
#include "Inv_ItemFragment.generated.h"

class UInv_CompositeBase;
/**
 * 物品片段的基类
 * 
 * 我们将道具的每个属性理解为“片段”Fragment
 *
 * 例如，道具名称，道具堆叠数量，道具占用格子的大小，道具价值，道具重量，道具描述，道具伤害，道具属性之类的，我们将其拆分成一个个的Fragment
 *
 * 对于不同的道具我们可以随意的灵活进行组合
 *
 * 希望每个物品片段都是可识别的
 * 
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

	
	//---End 特殊成员函数---
	
	//如果这个结构类型的子类被多态地销毁
	//也就是说，通过父类类型的指针，那么父类的析构函数将被调用，并且所有继承链中的析构函数都会依次执行
	//因此，在处理继承时，将析构函数设为虚函数是很重要的

	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }
	
private:

	//用来给道具赋予片段属性的字段，使用GameplayTag来进行区分
	UPROPERTY(EditAnywhere, Category="属性设置", meta=(Categories="FragmentTags"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
	
};

/**
 * Item fragment specifically for assimilation into a widget
 * 专门用于同化到小部件中的项片段
 */
USTRUCT(BlueprintType)
struct FInv_InventoryItemFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UInv_CompositeBase* Composite) const;

protected:
	bool MatchesWidgetTag(const UInv_CompositeBase* Composite) const;
	
};

/**
 * 道具片段：占用格子相关的信息
 *
 * GridSize 道具在道具栏中占用多少格子
 *
 * GridPadding 边距
 */
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

/**
 * 属性片段：道具图标
 */
USTRUCT(BlueprintType)
struct FInv_ImageFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	UTexture2D* GetIcon() const { return IconTexture; }

private:

	//图标的贴图纹理
	UPROPERTY(EditAnywhere, Category="属性设置")
	TObjectPtr<UTexture2D> IconTexture{nullptr};

	//图标的大小
	UPROPERTY(EditAnywhere, Category="属性设置")
	FVector2D IconDimensions{44.f, 44.f};
	
};

/**
 * 属性片段：道具数量
 */
USTRUCT(BlueprintType)
struct FInv_StackableFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	int32 GetMaxStackSize() const { return MaxStackSize; }
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }

private:

	UPROPERTY(EditAnywhere, Category="属性设置")
	int32 MaxStackSize{1};//最大堆叠数量

	UPROPERTY(EditAnywhere, Category="属性设置")
	int32 StackCount{1};//当前堆叠数量
	
};

/**
 * 作为消耗品功能的父类结构体
 * 使用之后不同的功能，需要重写Consume方法
 */
USTRUCT()
struct FInv_ConsumableFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) {}
};

/**
 * 继承自FInv_ConsumableFragment
 * 实现生命值药品回复的效果
 */
USTRUCT()
struct FInv_HealthPotionFragment : public FInv_ConsumableFragment
{
	GENERATED_BODY()

	//回复的生命值数量
	UPROPERTY(EditAnywhere, Category="属性设置")
	float  HealAmount = 20.f;
	
	virtual void OnConsume(APlayerController* PC) override;
	
};

/**
 * 继承自FInv_ConsumableFragment
 * 实现法力值药品回复的效果
 */
USTRUCT()
struct FInv_ManaPotionFragment : public FInv_ConsumableFragment
{
	GENERATED_BODY()

	//回复的法力值数量
	UPROPERTY(EditAnywhere, Category="属性设置")
	float  ManaAmount = 20.f;
	
	virtual void OnConsume(APlayerController* PC) override;
	
};

