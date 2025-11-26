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

	virtual void Collapse();//用于折叠复合组件，将其设置为不可见状态
	void Expand();//用于展开复合组件，使其变为可见状态

	using FUncType = TFunction<void(UInv_CompositeBase*)>;
	virtual void ApplyFunction(FUncType Function) {}

protected:

private:

	UPROPERTY(EditAnywhere, Category="Inventory")
	FGameplayTag FragmentTag;
	
};
