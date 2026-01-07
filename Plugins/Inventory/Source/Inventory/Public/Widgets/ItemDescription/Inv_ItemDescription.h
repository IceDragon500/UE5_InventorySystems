// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/Inv_Composite.h"
#include "Inv_ItemDescription.generated.h"

class USizeBox;
/**
 * 显示鼠标指向道具弹出的提示窗口
 */
UCLASS()
class INVENTORY_API UInv_ItemDescription : public UInv_Composite
{
	GENERATED_BODY()

public:

	FVector2D GetBoxSize() const;
	void SetBoxSize(FVector2D BoxSize);
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

protected:

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
