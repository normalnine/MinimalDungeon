// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MD_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API UMD_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float swordDmg = 0;

	UPROPERTY()
	float swordCrit = 0;
	
	UPROPERTY()
	float knifeDmg = 0;

	UPROPERTY()
	float knifeCrit = 0;

	UPROPERTY()
	float Evasion = 0;	
};
