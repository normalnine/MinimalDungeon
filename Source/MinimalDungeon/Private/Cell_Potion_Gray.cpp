// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell_Potion_Gray.h"
#include "MD_GameInstance.h"
#include <Components/TextRenderComponent.h>
#include "VR_Player.h"


ACell_Potion_Gray::ACell_Potion_Gray()
{
	price = 5;
	probName = "Gray Potion";
	probEffect = "Evasion +1";
}

void ACell_Potion_Gray::PotionEffect()
{	
	gameInst->evasion += 1;
}