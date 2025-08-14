// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;


/**
 * 
 */
UCLASS()
class INVENTORY_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:

	void PrimaryInteract();

	UPROPERTY(EditDefaultsOnly, Category="属性设置|Inventory")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	UPROPERTY(EditDefaultsOnly, Category="属性设置|Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;
};
