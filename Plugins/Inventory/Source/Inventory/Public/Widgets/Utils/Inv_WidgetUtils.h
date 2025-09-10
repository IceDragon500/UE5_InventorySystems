// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Inv_WidgetUtils.generated.h"

/**
 * 创建几个实用函数用来辅助计算
 */
UCLASS()
class INVENTORY_API UInv_WidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 根据传入物品在物品栏中的坐标（行 列） 得到物品在道具列表中的Index索引
	 * @param Position 道具在道具栏中的坐标
	 * @param Columns 道具栏总共有多少列
	 * @return 道具在道具列表中的Index索引 从1~N
	 */
	static int32 GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);

	/**
	 * 根据道具的索引Index和所在列，得到道具在道具栏中的坐标
	 * @param Index 道具在道具列表中的索引Index
	 * @param Columns 道具栏总共有多少列
	 * @return 道具在道具栏中的坐标（行 列）
	 */
	static FIntPoint GetPositionFromIndex(int32 Index, const int32 Columns);
};
