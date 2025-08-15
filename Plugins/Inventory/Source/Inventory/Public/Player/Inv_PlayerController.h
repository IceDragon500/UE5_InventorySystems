// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

class UInv_HUDWidget;
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
	AInv_PlayerController();
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:

	//
	void PrimaryInteract();

	//创建屏幕上显示的内容
	void CreateHUDWidget();

	//针对物品的射线检测
	void TraceForItem();

	UPROPERTY(EditDefaultsOnly, Category="属性设置|Inventory")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	UPROPERTY(EditDefaultsOnly, Category="属性设置|Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category="属性设置|Inventory")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;

	//道具检测的距离
	UPROPERTY(EditDefaultsOnly, Category="属性设置|Inventory")
	double TranceLength {1000.f};

	//用来设置自定义的通道检测类型
	UPROPERTY(EditDefaultsOnly, Category="属性设置|Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel {ECollisionChannel::ECC_Camera} ;

	TWeakObjectPtr<AActor> ThisActor;//当前指向的Actor
	TWeakObjectPtr<AActor> LastActor;//最后一次指向的Actor
	
};
