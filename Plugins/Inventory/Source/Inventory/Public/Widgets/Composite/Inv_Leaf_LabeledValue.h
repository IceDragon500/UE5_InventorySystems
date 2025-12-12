// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inv_Leaf.h"
#include "Inv_Leaf_LabeledValue.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_Leaf_LabeledValue : public UInv_Leaf
{
	GENERATED_BODY()

public:

	void SetLabelText(const FText& Text, bool bCollapse) const;
	void SetValueText(const FText& Text, bool bCollapse) const;
	virtual void NativePreConstruct() override; 

protected:

private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Label;//标签

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Value;//数值

	UPROPERTY(EditAnywhere, Category="属性设置")
	int32 FontSize_Label{12};//标签字体大小

	UPROPERTY(EditAnywhere, Category="属性设置")
	int32 FontSize_Value{18};//数值字体大小
};
