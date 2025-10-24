// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemPopUp.generated.h"

class USizeBox;
class UTextBlock;
class USlider;
class UButton;
/**
 * 当在物品栏网格中右键点击物品时，物品弹出窗口会显示出来
 */
UCLASS()
class INVENTORY_API UInv_ItemPopUp : public UUserWidget
{
	GENERATED_BODY()

public:

protected:

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Split; //分割物品按钮

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Drop; //丢弃物品按钮

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consume; //使用物品按钮

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> Slider_Split; //分割量的滑块

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Split; //显示分割出来的数量

	
};
