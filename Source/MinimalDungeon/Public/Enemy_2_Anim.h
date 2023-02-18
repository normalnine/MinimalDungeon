// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy_2_FSM.h"
#include "Enemy_2_Anim.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API UEnemy_2_Anim : public UAnimInstance
{
	GENERATED_BODY()
	public:
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		EEnemy2State animState;
		//�ǰ� �ִϸ��̼� ��� �Լ�
		UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
			void PlayDamageAnim(FName sectionName);
};
