// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_1_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	AttackDelay,
	Damage,
	Die,
	ReturnPos
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIMALDUNGEON_API UEnemy_1_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemy_1_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState mState = EEnemyState::Idle;

	//대기상태
	void IdleState();
	//이동상태
	void MoveState();
	//공격상태
	void AttackState();
	//공격 대기
	void UpdaetAttackDelay();
	//피격상태
	void DamageState();
	//죽음상태
	void DieState();
	//리턴
	void UpdateReturnPos();


	//대기시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 1;
	//경과시간
	float currentTime = 0;
	//타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AVR_Player* target;
	//소유 액터
	UPROPERTY()
		class AEnemy_1* me;
		
};
