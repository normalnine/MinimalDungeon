// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell_Potion_Blue.h"
#include "MD_GameInstance.h"
#include <Components/TextRenderComponent.h>
#include "VR_Player.h"


ACell_Potion_Blue::ACell_Potion_Blue()
{
	price = 5;
	probName = "Blue Potion";
	probEffect = "Knife Crit +3";
}

void ACell_Potion_Blue::PotionEffect()
{
	gameInst->knifeCrit += 3;
}

