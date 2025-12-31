// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Inv_EquipActor.generated.h"


UCLASS()
class INVENTORY_API AInv_EquipActor : public AActor
{
	GENERATED_BODY()

public:

	AInv_EquipActor();
	FGameplayTag GetEquipmentTypeTag() const { return EquipmentType; }
	void SetEquipmentTag(FGameplayTag Type) { EquipmentType = Type; }

protected:

private:

	UPROPERTY(EditAnywhere, Category="属性设置")
	FGameplayTag EquipmentType;
	

};
