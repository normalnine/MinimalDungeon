// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_2_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemy2State : uint8
{
	Idle,
	Attack,
	AttackDelay,
	Damage,
	Die
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIMALDUNGEON_API UEnemy_2_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemy_2_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemy2State mState = EEnemy2State::Idle;

	//대기상태
	void IdleState();
	//공격상태
	void AttackState();
	//공격 대기
	void UpdaetAttackDelay();
	//피격상태
	void DamageState();
	//죽음상태
	void DieState();
	//피격당했을때
	UFUNCTION(BlueprintCallable)
	void OnDamageProcess(int32 damage);

	//타겟
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AVR_Player* target;

	UPROPERTY()
		class AEnemy_2* me;

	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 1500.0f;

	UPROPERTY(EditAnywhere, Category = FSM)
		float currentTime = 0;

	UPROPERTY(EditAnywhere, Category = FSM)
		float DelayTime = 2.0f;
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class AEnemy_2_Bullet> bulletFactory;

	//체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 2;

	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float dieSpeed = 50.0f;

	//색바꾸려는 변수
	class UMaterialInstanceDynamic* mat2;

	//원래색으로 돌아가는 함수
	void ColorOff();
	//색깔 나오는 시간 핸들
	FTimerHandle colorHandle;
	UPROPERTY()
		class UEnemy_2_Anim* anim;

	//드랍시킬아이템
	UPROPERTY(EditDefaultsOnly, Category = DropFactory)
		TSubclassOf<class ACoin> DropFactory;
};
