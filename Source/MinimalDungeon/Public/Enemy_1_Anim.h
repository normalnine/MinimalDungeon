// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy_1_FSM.h"
#include "Enemy_1_Anim.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API UEnemy_1_Anim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category=FSM)
	EEnemyState animState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool battackPlay = false;

		UFUNCTION(BlueprintCallable,Category=FSMEvent)
		void OnEndAttackAnimation();
		//피격 애니메이션 재생 함수
		UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
			void PlayDamageAnim(FName sectionName);
		//죽음 상태 애니메이션 종료 여부
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
			bool bDieDone = false;
};
