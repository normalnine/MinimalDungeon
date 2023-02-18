// Fill out your copyright notice in the Description page of Project Settings.


#include "CellProbUI.h"
#include <UMG/Public/Components/TextBlock.h>

void UCellProbUI::UpdateInfo(int32 price, FString name, FString effect)
{
	
	_price->SetText(FText::AsNumber(price));
	_probname->SetText(FText::FromString(name));
	_probeffect->SetText(FText::FromString(effect));
}
