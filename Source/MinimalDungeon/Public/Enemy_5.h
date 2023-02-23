// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyF.h"
#include "Enemy_5.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API AEnemy_5 : public AEnemyF
{
	GENERATED_BODY()
	AEnemy_5();
	public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
		class UEnemy_5_FSM* fsm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool dam = false;

};
