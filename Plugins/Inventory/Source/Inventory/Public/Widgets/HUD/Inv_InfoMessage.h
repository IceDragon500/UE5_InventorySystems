// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_InfoMessage.generated.h"

class UTextBlock;


/**
 * 专门用来显示信息的窗口控件
 * 触发某些事件时，需要弹出一些消息提示框
 * 例如背包满了 当前无法进行动作 之类的提示
 * 需要可以设置提示的文字TextBlock
 * 需要设置消息存在的时长MessageLifeTime
 */
UCLASS()
class INVENTORY_API UInv_InfoMessage : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category="属性设置")
	void MessageShow();

	UFUNCTION(BlueprintImplementableEvent, Category="属性设置")
	void MessageHide();

	void SetMessage(const FText& Message);
	
protected:
	
private:
	//设置提示的文字
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock;

	//设置消息存在的时长
	UPROPERTY(EditAnywhere, Category="属性设置")
	float MessageLifeTime {3.f};

	FTimerHandle MessageHandle;

	//消息当前是否被激活 避免被反复激活消息
	bool bIsMessageActive {false};
};
