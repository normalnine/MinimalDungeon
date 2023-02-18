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
	UPROPERTY(EditAnywhere)
	int32 swordDmg = 6;

	UPROPERTY(EditAnywhere)
	int32 swordCrit = 5;
	
	UPROPERTY(EditAnywhere)
	int32 knifeDmg = 3;

	UPROPERTY(EditAnywhere)
	int32 knifeCrit = 5;

	UPROPERTY(EditAnywhere)
	int32 evasion = 0;

	UPROPERTY(EditAnywhere)
	int32 hp = 10;

	UPROPERTY(EditAnywhere)
	int32 key = 0;

	UPROPERTY(EditAnywhere)
	int32 coin = 0;
};
