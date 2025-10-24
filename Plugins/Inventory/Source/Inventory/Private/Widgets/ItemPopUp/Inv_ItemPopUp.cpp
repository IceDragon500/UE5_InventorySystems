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

void UInv_ItemPopUp::OnSplitButtonClicked()
{
}

void UInv_ItemPopUp::OnDropButtonClicked()
{
}

void UInv_ItemPopUp::OnConsumeButtonClicked()
{
}

void UInv_ItemPopUp::OnSplitValueChanged(float Value)
{
}
