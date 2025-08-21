// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Inv_InventoryItem.generated.h"

/**
 * 作为道具的父类
 * 每一个道具分类，都可以从这里进行继承
 */
UCLASS()
class INVENTORY_API UInv_InventoryItem : public UObject
{
	GENERATED_BODY()
};
