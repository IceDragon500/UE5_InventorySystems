// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Fragments/Inv_ItemFragment.h"

#include "EquipmentManagement/EquipActor/Inv_EquipActor.h"
#include "Widgets/Composite/Inv_CompositeBase.h"
#include "Widgets/Composite/Inv_Leaf_Image.h"
#include "Widgets/Composite/Inv_Leaf_LabeledValue.h"
#include "Widgets/Composite/Inv_Leaf_Text.h"

void FInv_InventoryItemFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->Expand();
}

bool FInv_InventoryItemFragment::MatchesWidgetTag(const UInv_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

void FInv_ImageFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_Image* Image = Cast<UInv_Leaf_Image>(Composite);
	if (!IsValid(Image)) return;

	Image->SetImage(IconTexture);
	Image->SetBoxSize(IconDimensions);
	Image->SetImageSize(IconDimensions);
}

void FInv_TextFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_Text* LeafText = Cast<UInv_Leaf_Text>(Composite);
	if (!IsValid(LeafText)) return;

	LeafText->SetText(FragmentText);
}

void FInv_LabeledNumberFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_LabeledValue* LabeledValue = Cast<UInv_Leaf_LabeledValue>(Composite);
	if (!IsValid(LabeledValue)) return;

	LabeledValue->SetLabelText(Text_Label, bCollapseLabel);

	FNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = MinFractionalDigits;
	Options.MaximumFractionalDigits = MaxFractionalDigits;

	LabeledValue->SetValueText(FText::AsNumber(Value, &Options), bCollapseValue);
}

void FInv_LabeledNumberFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();

	if (bRandomizeOnManifest)
	{
		Value = FMath::FRandRange(ValueMin, ValueMax);
	}

	bRandomizeOnManifest = false;
}

void FInv_ConsumableFragment::OnConsume(APlayerController* PC)
{
	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnConsume(PC);
	}
}

void FInv_ConsumableFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier : ConsumeModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInv_ConsumableFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();

	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}

void FInv_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	//展示：从玩家控制器或 PC 那里获取一个状态组件你可以从中获取生命值

	//获取能力系统组件并应用一个游戏效果

	//放置一个能力或调用接口函数来实现治疗

	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Green,
		FString::Printf(TEXT(" %f 点生命值被添加了! "),GetValue()));
}

void FInv_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Blue,
		FString::Printf(TEXT(" %f 点法力值被添加了! "), GetValue()));
}

void FInv_StrengthModifier::OnEquip(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT(" %f 点力量值生效了! "), GetValue()));
}

void FInv_StrengthModifier::OnUnEquip(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT(" %f 点力量值失效了! "), GetValue()));
}

void FInv_EquipmentFragment::OnEquip(APlayerController* PC)
{
	if (bEquipped) return;
	bEquipped = true;
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnEquip(PC);
	}
}

void FInv_EquipmentFragment::OnUnEquip(APlayerController* PC)
{
	if (!bEquipped) return;
	bEquipped = false;
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnUnEquip(PC);
	}
}

void FInv_EquipmentFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier : EquipModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInv_EquipmentFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}

AInv_EquipActor* FInv_EquipmentFragment::SpawnAttachedActor(USkeletalMeshComponent* AttachMesh) const
{
	if (!IsValid(EquipActorClass) || !IsValid(AttachMesh)) return nullptr;

	AInv_EquipActor* SpawnActor = AttachMesh->GetWorld()->SpawnActor<AInv_EquipActor>(EquipActorClass);
	SpawnActor->AttachToComponent(AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketAttachPoint);

	return SpawnActor;
	
}

void FInv_EquipmentFragment::DestroyAttachedActor() const
{
	if (EquippedActor.IsValid())
	{
		EquippedActor->Destroy();
	}
}
