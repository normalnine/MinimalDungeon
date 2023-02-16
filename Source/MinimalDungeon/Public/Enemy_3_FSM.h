// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_3_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemy3State : uint8
{
	Idle,
	Move,
	Attack,
	AttackDelay,
	Damage,
	Die,
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIMALDUNGEON_API UEnemy_3_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemy_3_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemy3State mState = EEnemy3State::Idle;

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


	//���ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 1;
	//����ð�
	float currentTime = 0;

	//Ÿ��
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AVR_Player* target;
	//���� ����
	UPROPERTY()
		class AEnemy_3* me;

	//���ݹ���
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;

	//���ݴ��ð�
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;

	//�ǰ� �˸� �̺�Ʈ �Լ�
	UFUNCTION(BlueprintCallable)
		void OnDamageProcess(float damage);

	//ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 3;

	//�ǰ� ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float damageDelayTime = 2.0f;
	UPROPERTY()
		class UEnemy_3_Anim* anim;

	//AI
	UPROPERTY()
		class AAIController* ai;

	//���� ����� �ѹ���!!! ȣ�� �Ǵ� �Լ�
	void ChangeState(EEnemy3State state);

	//Ÿ���� �i�ư����ֳ�?
	bool IsTargetTrace();

	//���� �޾��� �� ó���ϴ� �Լ�
	void ReceiveDamage();

	//Delay �Լ�
	bool IsWaitComplete(float delayTime);

	//�ش� ��ġ���� �����ϸ� ���¸� Idle �� ��ȯ�ϴ� �Լ�
	void MoveToPos(FVector pos);

	//����ð�
	float currTime = 0;
	//�� ã�� ����� ���� ��ġ
	FVector randomPos;

	//���� ��ġ ��������
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	//ó����ġ�� ��Ƴ��� ����
	FVector originPos;

	//������ ��ġ
	FVector randPos;

	//�̵��Ҽ��ִ� ����
	float moveRange = 3000;

	//�Ѿ� �� �� �ִ� ����
	float traceRange = 1000;

	//anim montage
	UPROPERTY(EditAnywhere)
		class UAnimMontage* damageMontage;

	//���ٲٷ��� ����
	class UMaterialInstanceDynamic* mat;

	//���������� ���ư��� �Լ�
	void ColorOff();

	//���� ������ �ð� �ڵ�
	FTimerHandle colorHandle;


	UPROPERTY(EditDefaultsOnly, Category = EnemyFactory)
		TSubclassOf<class AEnemy_3> EnemyFactory;

};
