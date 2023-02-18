// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CellProbActor.h"
#include "Cell_Potion_Pink.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API ACell_Potion_Pink : public ACellProbActor
{
	GENERATED_BODY()

public:
	ACell_Potion_Pink();

	UFUNCTION()
	virtual void PotionEffect() override;

	
};
