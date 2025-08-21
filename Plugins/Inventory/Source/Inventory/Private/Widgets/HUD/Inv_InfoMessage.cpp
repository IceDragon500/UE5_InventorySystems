// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/Inv_InfoMessage.h"

#include "Components/TextBlock.h"

void UInv_InfoMessage::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock->SetText(FText::GetEmpty());
	MessageHide();
}

void UInv_InfoMessage::SetMessage(const FText& Message)
{
	TextBlock->SetText(Message);

	if (!bIsMessageActive)
	{
		MessageShow();
	}
	bIsMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(MessageHandle,
		[this]()
		{
			MessageHide();
			bIsMessageActive = false;
		},
		MessageLifeTime, false);
}
