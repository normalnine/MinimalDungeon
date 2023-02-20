// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpFoodUI.h"
#include <UMG/Public/Components/TextBlock.h>

void UPickUpFoodUI::UpdateInfo(FString foodName, FString foodEffect)
{
	_foodname->SetText(FText::FromString(foodName));
	_foodeffect->SetText(FText::FromString(foodEffect));
}
