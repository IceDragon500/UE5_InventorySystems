// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemComponent.generated.h"

/**
 * ItemComponent
 * 所有的物品都有一个包含所有属性的物品组件
 * 将该物品变成可拾取并添加到我们的库存中的物品
 * 我们可以将这个组件添加到任意Actor上，使该Actor成为我们可以追踪、击中并拾取的对象
 *
 * 这里 我们使用FInv_ItemManifest的结构体来设置道具的各种属性
 * 我们将道具的各种属性拆分成类似词条的东西，教程这里叫做片段Fragment
 * 每一个道具都可以使用不同的Fragment来达成不同的效果
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UInv_ItemComponent();

	FString GetPickupMessage() const { return PickupMessage; }

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FInv_ItemManifest GetItemManifest() const { return ItemManifest; }

protected:


private:

	UPROPERTY(EditAnywhere, Category="属性设置")
	FString PickupMessage {TEXT("默认信息")};

	//使用FInv_ItemManifest的结构体来设置道具的各种属性
	UPROPERTY(Replicated, EditAnywhere, Category="属性设置")
	FInv_ItemManifest ItemManifest;
	
};
