// Fill out your copyright notice in the Description page of Project Settings.


#include "MD_GameModeBase.h"

AMD_GameModeBase::AMD_GameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/KDH/Bluprints/BP_VR_Player.BP_VR_Player_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}