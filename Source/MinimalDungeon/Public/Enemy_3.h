// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyF.h"
#include "Enemy_3.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API AEnemy_3 : public AEnemyF
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AEnemy_3();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
		class UEnemy_3_FSM* fsm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		bool hiton = false;
};
