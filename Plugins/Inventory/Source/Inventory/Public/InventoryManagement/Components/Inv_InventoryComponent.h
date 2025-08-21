// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_InventoryComponent.generated.h"
class UInv_ItemComponent;
class UInv_InventoryBase;
class UInv_InventoryItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FinventoryItemChange, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);


/**
 * 道具管理组件
 * 支持道具的各种操作
 * 支持界面上相关的功能，例如不同类型道具的界面切换
 * 与所属玩家控制器PlayerController进行关联
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UInv_InventoryComponent();

	/**
	 * 尝试进行道具的添加
	 * 如果添加失败，比如背包满了，我们希望广播一个委托，用于当我们的库存组件因为满了而失败的情况
	 * @param ItemComponent 
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	void TryAddItem(UInv_ItemComponent* ItemComponent);

	//切换库存菜单
	void ToggleInventoryMenu();

	FinventoryItemChange OnItemAdded;//在道具被添加的时候进行一个广播
	FinventoryItemChange OnItemRemoved;//在道具被删除的时候进行一个广播
	FNoRoomInInventory NoRoomInInventory;//添加道具失败的时候进行一个广播

protected:

	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<APlayerController> OwningController;

	//构造库存
	void ConstructInventory();

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UInv_InventoryBase> InventoryMenuClass;

	UPROPERTY()
	TObjectPtr<UInv_InventoryBase> InventoryMenu;

	bool bInventoryMenuOpen = false;
	void OpenInventoryMenu();
	void CloseInventoryMenu();
	
	
};
