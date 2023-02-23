// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsUI.h"
#include "MD_GameInstance.h"
#include <UMG/Public/Components/TextBlock.h>
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Button.h>

void UStatsUI::NativeConstruct()
{
	Super::NativeConstruct();

	_giveup->OnPressed.AddDynamic(this, &UStatsUI::GiveUp);
	_gomain->OnPressed.AddDynamic(this, &UStatsUI::GoMain);
}

void UStatsUI::UpdateStats()
{	
	gameInst = Cast<UMD_GameInstance>(GetWorld()->GetGameInstance());

	_swordDmg->SetText(FText::AsNumber(gameInst->swordDmg));

	FString strSwordCrit = FString::Printf(TEXT("%d%%"), gameInst->swordCrit);
	_swordCrit->SetText(FText::FromString(strSwordCrit));

	_knifeDmg->SetText(FText::AsNumber(gameInst->knifeDmg));

	FString strKnifdCrit = FString::Printf(TEXT("%d%%"), gameInst->knifeCrit);
	_knifeCrit->SetText(FText::FromString(strKnifdCrit));

	FString strEvasion = FString::Printf(TEXT("%d%%"), gameInst->evasion);
	_evasion->SetText(FText::FromString(strEvasion));
}

void UStatsUI::GiveUp()
{
	return;
}

void UStatsUI::GoMain()
{	
	gameInst->StatsInit();
}

