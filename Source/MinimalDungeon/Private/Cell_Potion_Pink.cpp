// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell_Potion_Pink.h"
#include "MD_GameInstance.h"
#include <Components/TextRenderComponent.h>
#include "VR_Player.h"

ACell_Potion_Pink::ACell_Potion_Pink()
{
	price = 5;
	probName = "Pink Potion";
	probEffect = "Sword Crit +5";
}

void ACell_Potion_Pink::PotionEffect()
{

	if (gameInst->coin >= price)
	{
		gameInst->swordCrit += 5;
		gameInst->coin -= price;
		GetWorld()->DestroyActor(this);
	}
}