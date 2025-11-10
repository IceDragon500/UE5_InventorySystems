// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Inv_CompositeBase.generated.h"


/**
 * 复合组件的基类
 */
UCLASS()
class INVENTORY_API UInv_CompositeBase : public UUserWidget
{
	GENERATED_BODY()

public:
	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(const FGameplayTag& Tag) { FragmentTag = Tag; }

	virtual void Collapse();//复合组件都能通过此函数实现折叠与展开
	void Expand();

	using FuncType = TFunction<void(UInv_CompositeBase*)>;
	virtual void ApplyFunction(FuncType Function) {}

protected:

private:

	UPROPERTY(EditAnywhere, Category="Inventory")
	FGameplayTag FragmentTag;
	
};
