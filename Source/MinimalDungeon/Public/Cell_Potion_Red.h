// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CellProbActor.h"
#include "Cell_Potion_Red.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API ACell_Potion_Red : public ACellProbActor
{
	GENERATED_BODY()
	
public:
	ACell_Potion_Red();
	UFUNCTION()
	virtual void PotionEffect() override;

};
