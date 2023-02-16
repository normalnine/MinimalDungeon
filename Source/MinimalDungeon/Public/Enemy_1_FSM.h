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

	//상태 변경시 한번만!!! 호출 되는 함수
	void ChangeState(EEnemyState state);

	//피격 알림 이벤트 함수
	UFUNCTION(BlueprintCallable)
	void OnDamageProcess(float dmaage);

	//타겠을 쫒아갈수있나?
	bool IsTargetTrace();

	//공격 받았을 때 처리하는 함수
	void ReceiveDamage();

	//Delay 함수
	bool IsWaitComplete(float delayTime);

	//해당 위치까지 도착하면 상태를 Idle 로 전환하는 함수
	void MoveToPos(FVector pos);

	//대기시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 1;
	//경과시간
	float currentTime = 0;

	//현재시간
	float currTime = 0;

	//타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AVR_Player* target;
	//소유 액터
	UPROPERTY()
		class AEnemy_1* me;
		
	//공격범위
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 200.0f;

	//쫓아 갈 수 있는 범위
	float traceRange = 1000;

	//공격대기시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;
	//체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 3;
	//피격 대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float damageDelayTime = 2.0f;
		//아래로 사라지는 속도
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float dieSpeed = 50.0f;
	//사용 중인 애니메이션 블루프린트
	UPROPERTY()
		class UEnemy_1_Anim* anim;
	//anim montage
	UPROPERTY(EditAnywhere)
		class UAnimMontage* damageMontage;
	//AI
	UPROPERTY()
		class AAIController* ai;
	//길 찾기 수행시 랜덤 위치
	FVector randomPos;

	//랜덤 위치 가져오기
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	//처음위치를 담아놓을 변수
	FVector originPos;

	//랜덤한 위치
	FVector randPos;

	//이동할수있는 범위
	float moveRange = 2000;

	//드랍시킬아이템
	UPROPERTY(EditDefaultsOnly, Category = DropFactory)
		TSubclassOf<class ACoin> DropFactory;

	//색바꾸려는 변수
	class UMaterialInstanceDynamic* mat;

	//원래색으로 돌아가는 함수
	void ColorOff();
	//색깔 나오는 시간 핸들
	FTimerHandle colorHandle;
};
