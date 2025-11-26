// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inv_CompositeBase.h"
#include "Inv_Composite.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_Composite : public UInv_CompositeBase
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void ApplyFunction(FUncType Function) override;
	virtual void Collapse() override;

protected:

private:

	UPROPERTY()
	TArray<TObjectPtr<UInv_CompositeBase>> Children;
	
};
