// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_4_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemy4State : uint8
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
class MINIMALDUNGEON_API UEnemy_4_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemy_4_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemy4State mState = EEnemy4State::Idle;

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
	//타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AVR_Player* target;
	//소유 액터
	UPROPERTY()
		class AEnemy_4* me;
	//경과시간
	float currentTime = 0;
	//공격범위
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;
	//공격대기시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;
	//체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 3;
	//피격 알림 이벤트 함수
	UFUNCTION(BlueprintCallable)
		void OnDamageProcess();
	//피격 대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float damageDelayTime = 2.0f;
	//사용 중인 애니메이션 블루프린트
	UPROPERTY()
		class UEnemy_4_Anim* anim;
	//AI
	UPROPERTY()
		class AAIController* ai;
	//길 찾기 수행시 랜덤 위치
	FVector randomPos;

	//랜덤 위치 가져오기
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

};
