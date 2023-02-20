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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 swordDmg = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 swordCrit = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 knifeDmg = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 knifeCrit = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 evasion = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 hp = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 key = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 coin = 10;

	UFUNCTION()
	void StatsInit();
};
