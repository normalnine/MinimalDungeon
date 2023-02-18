// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell_Potion_Red.h"
#include "MD_GameInstance.h"
#include <Components/TextRenderComponent.h>
#include "VR_Player.h"

ACell_Potion_Red::ACell_Potion_Red()
{
	price = 5;
	probName = "Red Potion";
	probEffect = "Sword Dmg +5";
}

void ACell_Potion_Red::PotionEffect()
{
	if (gameInst->coin >= price)
	{
		gameInst->swordDmg += 5;
		gameInst->coin -= price;
		GetWorld()->DestroyActor(this);
	}	
}
