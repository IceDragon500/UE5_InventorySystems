// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_EquipmentComponent.generated.h"


struct FInv_ItemManifest;
struct FInv_EquipmentFragment;
class AInv_EquipActor;
class UInv_InventoryItem;
class UInv_InventoryComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	//UInv_EquipmentComponent(); 152讲 去掉了这个构造函数
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

public:

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	TWeakObjectPtr<APlayerController> OwningPlayerController;
	
	TWeakObjectPtr<USkeletalMeshComponent> OwningSkeletalMesh;

	UPROPERTY()
	TArray<TObjectPtr<AInv_EquipActor>> EquippedActors;

	UFUNCTION()
	void OnItemEquipped(UInv_InventoryItem* EquippedItem);

	UFUNCTION()
	void OnItemUnEquipped(UInv_InventoryItem* UnEquippedItem);

	void InitInventoryComponent();

	AInv_EquipActor* SpawnEquippedActor(FInv_EquipmentFragment* EquipmentFragment, const FInv_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh);


};
