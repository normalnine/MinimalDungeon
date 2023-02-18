// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell_Potion_Green.h"
#include "MD_GameInstance.h"
#include <Components/TextRenderComponent.h>
#include "VR_Player.h"
 
ACell_Potion_Green::ACell_Potion_Green()
{
	price = 5;
	probName = "Green Potion";
	probEffect = "Knife Dmg +5";
}

void ACell_Potion_Green::PotionEffect()
{
	if (gameInst->coin >= price)
	{
		gameInst->knifeDmg += 5;
		gameInst->coin -= price;
		GetWorld()->DestroyActor(this);
	}
}
