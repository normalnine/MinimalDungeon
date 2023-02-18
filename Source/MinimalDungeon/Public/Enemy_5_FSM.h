// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_5_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemy5State : uint8
{
	Idle,
	Move,
	Attack,
	AttackDelay,
	Damage,
	Die,
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIMALDUNGEON_API UEnemy_5_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemy_5_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemy5State mState = EEnemy5State::Idle;

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

	UPROPERTY(EditAnywhere, Category = FSM)
		float idleDelayTime = 2;
	// 경과 시간
	float currentTime = 0;

	// 타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AVR_Player* target;

	// 소유 액터
	UPROPERTY()
		class AEnemy_5* me;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;

	// 공격 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;

	// 피격 알림 이벤트 함수
	void OnDamageProcess(int32 damage);

	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 3;
	// 피격 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float damageDelayTime = 2.0f;

	// 아래로 사라지는 속도
	UPROPERTY(EditAnywhere, Category = FSM)
		float dieSpeed = 50.0f;


	// Enemy를 소유하고 있는 AIController
	UPROPERTY()
		class AAIController* ai;

	// 길 찾기 수행시 랜덤 위치
	FVector randomPos;
	// 랜덤 위치 가져오기
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	
};
