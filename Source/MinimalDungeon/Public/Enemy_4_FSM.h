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
	//����
	void UpdateReturnPos();
	//����
	void isSleep();
	//Ÿ��
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AVR_Player* target;
	//���� ����
	UPROPERTY()
		class AEnemy_4* me;
	//����ð�
	float currentTime = 0;
	//���ݹ���
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 300.0f;
	//���ݴ��ð�
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;
	//ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 3;
	//�ǰ� �˸� �̺�Ʈ �Լ�
	UFUNCTION(BlueprintCallable)
		void OnDamageProcess(int32 damage);
	//�ǰ� ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float damageDelayTime = 2.0f;
	//��� ���� �ִϸ��̼� �������Ʈ
	UPROPERTY()
		class UEnemy_4_Anim* anim;
	//AI
	UPROPERTY()
		class AAIController* ai;
	//�� ã�� ����� ���� ��ġ
	FVector randomPos;

	//���� ��ġ ��������
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	//Ÿ���� �i�ư����ֳ�?
	bool IsTargetTrace();

	//�Ѿ� �� �� �ִ� ����
	float traceRange = 1000;


	//���� ����� �ѹ��� ȣ�� �Ǵ� �Լ�
	void ChangeState(EEnemy4State state);

	//Delay �Լ�
	bool IsWaitComplete(float delayTime);

	//���ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 1;

	//ó����ġ�� ��Ƴ��� ����
	FVector originPos;

	//������ ��ġ
	FVector randPos;

	//�ش� ��ġ���� �����ϸ� ���¸� Idle �� ��ȯ�ϴ� �Լ�
	void MoveToPos(FVector pos);

	//����ð�
	float currTime = 0;

	//���ٲٷ��� ����
	class UMaterialInstanceDynamic* mat;

	//���������� ���ư��� �Լ�
	void ColorOff();

	//���� ������ �ð� �ڵ�
	FTimerHandle colorHandle;

	//�����ų������
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
