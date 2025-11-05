// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/Inv_ItemComponent.h"

#include "Net/UnrealNetwork.h"


UInv_ItemComponent::UInv_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInv_ItemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

void UInv_ItemComponent::InitItemManifest(FInv_ItemManifest CopyManifest)
{
	ItemManifest = CopyManifest;
}

void UInv_ItemComponent::PickedUp()
{
	OnPickedUp();
	
	//拾取物品后 摧毁这个物品
	GetOwner()->Destroy();

	//如果我们还想添加其他的特效或者其他的事情，我们可以创建一个蓝图可实现事件，并且在拾取函数中调用它
}

