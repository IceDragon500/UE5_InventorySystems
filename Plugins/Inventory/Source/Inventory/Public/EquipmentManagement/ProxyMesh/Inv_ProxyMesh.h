// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inv_ProxyMesh.generated.h"

class UInv_EquipmentComponent;

UCLASS()
class INVENTORY_API AInv_ProxyMesh : public AActor
{
	GENERATED_BODY()

public:

	AInv_ProxyMesh();

protected:

	virtual void BeginPlay() override;

private:

	//这是玩家控制的网格
	TWeakObjectPtr<USkeletalMeshComponent> SourceMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInv_EquipmentComponent> EquipmentComponent;

	//这是将在库存中看到的代理网格
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	
};
