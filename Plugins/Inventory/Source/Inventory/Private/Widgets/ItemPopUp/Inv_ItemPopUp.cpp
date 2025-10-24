// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemPopUp/Inv_ItemPopUp.h"

#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UInv_ItemPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Split->OnClicked.AddDynamic(this, &UInv_ItemPopUp::OnSplitButtonClicked);
	Button_Drop->OnClicked.AddDynamic(this, &UInv_ItemPopUp::OnDropButtonClicked);
	Button_Consume->OnClicked.AddDynamic(this, &UInv_ItemPopUp::OnConsumeButtonClicked);
	Slider_Split->OnValueChanged.AddDynamic(this, &UInv_ItemPopUp::OnSplitValueChanged);
}

void UInv_ItemPopUp::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();
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
	Text_SplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}

void UInv_ItemPopUp::CollapseSplitButton() const
{
	Button_Split->SetVisibility(ESlateVisibility::Collapsed);
	Slider_Split->SetVisibility(ESlateVisibility::Collapsed);
	Text_SplitAmount->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemPopUp::CollapseConsumeButton() const
{
	Button_Consume->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemPopUp::SetSliderParams(const float Max, const float Value) const
{
	Slider_Split->SetMaxValue(Max);
	Slider_Split->SetMinValue(1);
	Slider_Split->SetValue(Value);
	Text_SplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}

FVector2D UInv_ItemPopUp::GetBoxSize() const
{
	return FVector2D(SizeBox_Root->GetWidthOverride(), SizeBox_Root->GetHeightOverride());
}
