// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_4_FSM.h"
#include "Animation/AnimInstance.h"
#include "Enemy_4_Anim.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API UEnemy_4_Anim : public UAnimInstance
{
	GENERATED_BODY()
public:
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		EEnemy4State animState;
		//�ǰ� �ִϸ��̼� ��� �Լ�
		UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
			void PlayDamageAnim(FName sectionName);
};
