// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagement/Components/Inv_EquipmentComponent.h"

#include "EquipmentManagement/EquipActor/Inv_EquipActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"


void UInv_EquipmentComponent::SetOwningSkeletalMesh(USkeletalMeshComponent* OwningMesh)
{
	OwningSkeletalMesh = OwningMesh;
}

void UInv_EquipmentComponent::InitializeOwner(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		OwningPlayerController = PlayerController;
	}
	InitInventoryComponent();
}

void UInv_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	//如果我们拥有一个所有者角色，那么我们将设置该骨骼网格并调用初始化库存组件，在其中绑定（如果尚未绑定）

	/**
	 * 157讲
	 * 因为装备组件本身被放置在玩家控制器上
	 * 因此我们知道至少会获得一个有效的玩家控制器
	 * 我们只是还不确定它是否是一个角色
	 * 这就是为什么我们需要将一个委托绑定到我们的玩家控制器上，当它发生变化时，该委托将被广播
	 * 设置其pawn为我们默认的——在我们的游戏模式中设定
	 */

	InitPlayerController();
	
	
}

void UInv_EquipmentComponent::InitPlayerController()
{
	//我们正在将所有者转换为玩家控制器，并在执行操作前检查该所有者是否有效
	if (OwningPlayerController = Cast<APlayerController>(GetOwner()); OwningPlayerController.IsValid())
	{
		//现在我们要检查该玩家控制器所拥有的 Pawn 是否可以转换为角色类
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn()); IsValid(OwnerCharacter))
		{
			OnPossessedPawnChange(nullptr, OwnerCharacter);
		}
		else //如果失败，那么我将需要获取该玩家控制器并绑定到其上的一个委托
		{
			OwningPlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChange);
		}
	}
}

void UInv_EquipmentComponent::OnPossessedPawnChange(APawn* OldPawn, APawn* NewPawn)
{
	//我们在这里再检查一次
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn()); IsValid(OwnerCharacter))
	{
		OwningSkeletalMesh = OwnerCharacter->GetMesh();
	}

	InitInventoryComponent();
}

void UInv_EquipmentComponent::InitInventoryComponent()
{
	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(OwningPlayerController.Get());
	if (!InventoryComponent.IsValid()) return;

	if (!InventoryComponent->OnItemEquipped.IsAlreadyBound(this, &ThisClass::OnItemEquipped))
	{
		InventoryComponent->OnItemEquipped.AddDynamic(this, &ThisClass::OnItemEquipped);
	}

	if (!InventoryComponent->OnItemUnEquipped.IsAlreadyBound(this, &ThisClass::OnItemUnEquipped))
	{
		InventoryComponent->OnItemUnEquipped.AddDynamic(this, &ThisClass::OnItemUnEquipped);
	}
}

AInv_EquipActor* UInv_EquipmentComponent::SpawnEquippedActor(FInv_EquipmentFragment* EquipmentFragment,
	const FInv_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh)
{
	AInv_EquipActor* SpawnEquipActor = EquipmentFragment->SpawnAttachedActor(AttachMesh);
	SpawnEquipActor->SetEquipmentTag(EquipmentFragment->GetEquipmentTag());
	SpawnEquipActor->SetOwner(GetOwner());
	EquipmentFragment->SetEquippedActor(SpawnEquipActor);

	return SpawnEquipActor;	
}

AInv_EquipActor* UInv_EquipmentComponent::FindEquippedActor(const FGameplayTag& EquipmentTypeTag)
{
	auto FoundActor = EquippedActors.FindByPredicate([&EquipmentTypeTag](const AInv_EquipActor* EquippedActor)
	{
		return EquippedActor->GetEquipmentTypeTag().MatchesTagExact(EquipmentTypeTag);
	});

	return FoundActor ? *FoundActor : nullptr ;
}

void UInv_EquipmentComponent::RemoveEquippedActor(const FGameplayTag& EquipmentTypeTag)
{
	if (AInv_EquipActor* EquipActor = FindEquippedActor(EquipmentTypeTag); IsValid(EquipActor))
	{
		EquippedActors.Remove(EquipActor);
		EquipActor->Destroy();
	}
}

void UInv_EquipmentComponent::OnItemEquipped(UInv_InventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return; //不需要检查有效性 除非我们拥有有效的玩家控制器，否则不会执行任何这些操作 但我们确实需要检查的是权限是否具备
	if (!OwningSkeletalMesh.IsValid()) return;

	FInv_ItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_EquipmentFragment>();
	if (!EquipmentFragment) return;

	if(!bIsProxy) EquipmentFragment->OnEquip(OwningPlayerController.Get());
	

	AInv_EquipActor* SpawnedEquipActor = SpawnEquippedActor(EquipmentFragment, ItemManifest, OwningSkeletalMesh.Get());

	EquippedActors.Add(SpawnedEquipActor);
}


void UInv_EquipmentComponent::OnItemUnEquipped(UInv_InventoryItem* UnEquippedItem)
{
	if (!IsValid(UnEquippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return; //不需要检查有效性 除非我们拥有有效的玩家控制器，否则不会执行任何这些操作 但我们确实需要检查的是权限是否具备

	FInv_ItemManifest& ItemManifest = UnEquippedItem->GetItemManifestMutable();
	FInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_EquipmentFragment>();
	if (!EquipmentFragment) return;

	if(!bIsProxy) EquipmentFragment->OnUnEquip(OwningPlayerController.Get());

	RemoveEquippedActor(EquipmentFragment->GetEquipmentTag());

	
}







