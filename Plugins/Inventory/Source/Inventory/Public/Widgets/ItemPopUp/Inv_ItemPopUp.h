// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemPopUp.generated.h"

class USizeBox;
class UTextBlock;
class USlider;
class UButton;

DECLARE_DYNAMIC_DELEGATE_TwoParams(FPopUpMenuSplit, int32, SplitAmount, int32, Index); //99课这里删除了MULTICAST
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPopUpMenuDrop, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPopUpMenuConsume, int32, Index);


/**
 * 当在物品栏网格中右键点击物品时，物品弹出窗口会显示出来
 */
UCLASS()
class INVENTORY_API UInv_ItemPopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	FPopUpMenuSplit OnPopUpMenuSplit;
	FPopUpMenuDrop OnPopUpMenuDrop;
	FPopUpMenuConsume OnPopUpMenuConsume;

	int32 GetSplitAmount() const;

	void CollapseSplitButton() const;//设置拆分功能不可显示
	void CollapseConsumeButton() const;//设置使用功能不可显示

	void SetSliderParams(const float Max, const float Value) const;//设置拆分条的最大和最小值

	//获取弹出菜单界面的大小
	FVector2D GetBoxSize() const;

	void SetGridIndex(int32 Index) { GridIndex = Index; }
	int32 GetGridIndex() const { return GridIndex; }

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
	TObjectPtr<UTextBlock> Text_SplitAmount; //显示分割出来的数量

	int32 GridIndex{INDEX_NONE};

	UFUNCTION()
	void OnSplitButtonClicked();//分割物品按钮被按下

	UFUNCTION()
	void OnDropButtonClicked();//丢弃物品按钮被按下

	UFUNCTION()
	void OnConsumeButtonClicked();//使用物品按钮被按下

	UFUNCTION()
	void OnSplitValueChanged(float Value);//滑块值改变	
};


