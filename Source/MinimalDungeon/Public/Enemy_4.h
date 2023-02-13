// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyF.h"
#include "Enemy_4.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API AEnemy_4 : public AEnemyF
{
	GENERATED_BODY()
public:
		AEnemy_4();
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
			class UEnemy_4_FSM* fsm;
};
