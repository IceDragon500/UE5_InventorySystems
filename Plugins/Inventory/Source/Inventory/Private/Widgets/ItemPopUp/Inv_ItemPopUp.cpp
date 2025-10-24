// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemPopUp/Inv_ItemPopUp.h"

#include "Components/Button.h"
#include "Components/Slider.h"

void UInv_ItemPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Split->OnClicked.AddDynamic(this, &UInv_ItemPopUp::OnSplitButtonClicked);
	Button_Drop->OnClicked.AddDynamic(this, &UInv_ItemPopUp::OnDropButtonClicked);
	Button_Consume->OnClicked.AddDynamic(this, &UInv_ItemPopUp::OnConsumeButtonClicked);
	Slider_Split->OnValueChanged.AddDynamic(this, &UInv_ItemPopUp::OnSplitValueChanged);
}

int32 UInv_ItemPopUp::GetSplitAmount() const
{
	return FMath::Floor(Slider_Split->GetValue());
}

void UInv_ItemPopUp::OnSplitButtonClicked()
{
	//OnPopUpMenuSplit.Broadcast(GetSplitAmount(), GridIndex);
	//这是委托没有使用多播MULTICAST，这里的写法
	if (OnPopUpMenuSplit.ExecuteIfBound(GetSplitAmount(), GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUp::OnDropButtonClicked()
{
	OnPopUpMenuDrop.Broadcast(GridIndex);
	RemoveFromParent();
}

void UInv_ItemPopUp::OnConsumeButtonClicked()
{
	OnPopUpMenuConsume.Broadcast(GridIndex);
	RemoveFromParent();
}

void UInv_ItemPopUp::OnSplitValueChanged(float Value)
{
}
