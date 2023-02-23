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
	ReturnPos,
	sleep
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
		EEnemy4State mState= EEnemy4State::sleep;

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
	//수면
	void isSleep();
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
		float attackRange = 300.0f;
	//공격대기시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;
	//체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 3;
	//피격 알림 이벤트 함수
	UFUNCTION(BlueprintCallable)
		void OnDamageProcess(int32 damage);
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

	//타겠을 쫒아갈수있나?
	bool IsTargetTrace();

	//쫓아 갈 수 있는 범위
	float traceRange = 1000;


	//상태 변경시 한번만 호출 되는 함수
	void ChangeState(EEnemy4State state);

	//Delay 함수
	bool IsWaitComplete(float delayTime);

	//대기시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 1;

	//처음위치를 담아놓을 변수
	FVector originPos;

	//랜덤한 위치
	FVector randPos;

	//해당 위치까지 도착하면 상태를 Idle 로 전환하는 함수
	void MoveToPos(FVector pos);

	//현재시간
	float currTime = 0;

	//색바꾸려는 변수
	class UMaterialInstanceDynamic* mat;

	//원래색으로 돌아가는 함수
	void ColorOff();

	//색깔 나오는 시간 핸들
	FTimerHandle colorHandle;

	//드랍시킬아이템
	UPROPERTY(EditDefaultsOnly, Category = DropFactory)
		TSubclassOf<class ACoin> DropFactory;

		void destroyme();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
			bool dam = false;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
			int32 showdamage = 0;

		UPROPERTY(EditDefaultsOnly, Category = Sound)
			class USoundBase* hitonSound;

		UPROPERTY(EditDefaultsOnly, Category = Sound)
			class USoundBase* DieSound;
};
