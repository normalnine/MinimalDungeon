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

	//������
	void IdleState();
	//�̵�����
	void MoveState();
	//���ݻ���
	void AttackState();
	//���� ���
	void UpdaetAttackDelay();
	//�ǰݻ���
	void DamageState();
	//��������
	void DieState();

	UPROPERTY(EditAnywhere, Category = FSM)
		float idleDelayTime = 2;
	// ��� �ð�
	float currentTime = 0;

	// Ÿ��
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AVR_Player* target;

	// ���� ����
	UPROPERTY()
		class AEnemy_5* me;

	// ���� ����
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;

	// ���� ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;

	// �ǰ� �˸� �̺�Ʈ �Լ�
	void OnDamageProcess(int32 damage);

	// ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 3;
	// �ǰ� ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
		float damageDelayTime = 2.0f;

	// �Ʒ��� ������� �ӵ�
	UPROPERTY(EditAnywhere, Category = FSM)
		float dieSpeed = 50.0f;


	// Enemy�� �����ϰ� �ִ� AIController
	UPROPERTY()
		class AAIController* ai;

	// �� ã�� ����� ���� ��ġ
	FVector randomPos;
	// ���� ��ġ ��������
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	
};
