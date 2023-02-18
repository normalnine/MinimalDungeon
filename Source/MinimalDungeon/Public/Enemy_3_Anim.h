// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy_3_FSM.h"
#include "Enemy_3_Anim.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API UEnemy_3_Anim : public UAnimInstance
{
	GENERATED_BODY()

public:
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		EEnemy3State animState;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
			bool battackPlay = false;

		UFUNCTION(BlueprintCallable, Category = FSMEvent)
		void EndAttackAnimation();

		//�ǰ� �ִϸ��̼� ��� �Լ�
		UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
			void PlayDamageAnim(FName sectionName);

		//���� ���� �ִϸ��̼� ���� ����
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
			bool bDieDone = false;
};
