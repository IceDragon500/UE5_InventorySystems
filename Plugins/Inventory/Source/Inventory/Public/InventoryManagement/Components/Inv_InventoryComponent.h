// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_InventoryComponent.generated.h"
class UInv_InventoryBase;
class UInv_InventoryItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FinventoryItemChange, UInv_InventoryItem*, Item);


/**
 * 能支持多种类型的库存系统
 * 并且于所属玩家控制器PlayerController相关联
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UInv_InventoryComponent();

	//切换库存菜单
	void ToggleInventoryMenu();

	FinventoryItemChange OnItemAdded;
	FinventoryItemChange OnItemRemoved;

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
