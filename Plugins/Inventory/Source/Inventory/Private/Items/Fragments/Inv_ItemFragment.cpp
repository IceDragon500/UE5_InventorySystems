// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Fragments/Inv_ItemFragment.h"

void FInv_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	//展示：从玩家控制器或 PC 那里获取一个状态组件你可以从中获取生命值

	//获取能力系统组件并应用一个游戏效果

	//放置一个能力或调用接口函数来实现治疗

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(" %f Health Potion Used! "), HealAmount));
	
}

void FInv_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT(" %f Mana Potion Used! "), ManaAmount));
}
