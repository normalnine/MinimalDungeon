// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_5_FSM.h"
#include "VR_Player.h"
#include "Enemy_5.h"
#include <Kismet/GameplayStatics.h>
#include "MinimalDungeon.h"
#include <AIController.h>
#include <NavigationSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include <Engine/EngineTypes.h>
#include <Components/CapsuleComponent.h>
#include "GameFramework/Actor.h"
#include <Engine/World.h>

// Sets default values for this component's properties
UEnemy_5_FSM::UEnemy_5_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UEnemy_5_FSM::BeginPlay()
{
	Super::BeginPlay();


	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AVR_Player::StaticClass());

	target = Cast<AVR_Player>(actor);

	me = Cast<AEnemy_5>(GetOwner());

	ai = Cast<AAIController>(me->GetController());

	
}


// Called every frame
void UEnemy_5_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemy5State::Idle:
		IdleState();
		break;
	case EEnemy5State::Move:
		MoveState();
		break;
	case EEnemy5State::Attack:
		AttackState();
		break;
	case EEnemy5State::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemy5State::Damage:
		DamageState();
		break;
	case EEnemy5State::Die:
		DieState();
		break;


	}
}

void UEnemy_5_FSM::IdleState()
{
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	//UE_LOG(LogTemp, Warning, TEXT("idle"));
	// 2. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (currentTime > idleDelayTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("change move"));
		// 3. �̵� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemy5State::Move;
		// ��� �ð� �ʱ�ȭ 
		currentTime = 0;


		// ���� ������ ��ġ �����ֱ�
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UEnemy_5_FSM::MoveState()
{
	//UE_LOG(LogTemp, Warning, TEXT("start move"));
	// 1. Ÿ�� �������� �ʿ��ϴ�.
	FVector destination = target->GetActorLocation();
	// 2. ������ �ʿ��ϴ�.
	FVector dir = destination - me->GetActorLocation();
	// 3. �������� �̵��ϰ� �ʹ�.
	//me->AddMovementInput(dir.GetSafeNormal());
	// NavigationSystem ��ü ������
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	// ������ �� ã�� ��� ������ �˻�
	FPathFindingQuery query;
	FAIMoveRequest req;
	// ���������� ������ �� �ִ� ����
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	// �� ã�⸦ ���� ���� ����
	ai->BuildPathfindingQuery(req, query);
	// �� ã�� ��� ��������
	FPathFindingResult r = ns->FindPathSync(query);

	// ������ ������ ��ã�� ���� ���� Ȯ��
	if (r.Result == ENavigationQueryResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("moveto target"));
		// Ÿ�������� �̵�
		ai->MoveToLocation(destination);
	}
	else
	{
		// ���� ��ġ�� �̵�
		//UE_LOG(LogTemp, Warning, TEXT("moveto randompos"));
		auto result = ai->MoveToLocation(randomPos);
		// �������� �����ϸ�
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// ���ο� ���� ��ġ ��������
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}

	// Ÿ��� ��������� ���� ���·� ��ȯ�ϰ� �ʹ�.
	// 1. ���� �Ÿ��� ���� ���� �ȿ� ������
	if (dir.Size() < attackRange)
	{
		// �� ã�� ��� ����
		ai->StopMovement();
	//	UE_LOG(LogTemp, Warning, TEXT("change attack"));
		// 2. ���� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemy5State::Attack;

		// ���� ���� ��ȯ �� ��� �ð��� �ٷ� �������� ó�� 
		currentTime = attackDelayTime;
	}
}

void UEnemy_5_FSM::AttackState()
{
	//UE_LOG(LogTemp, Warning, TEXT("attack start"));
		// 1. �ð��� �귯�� �Ѵ�.
		currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���� �ð��� �����ϱ�
	if (currentTime > attackDelayTime)
	{
		// 3. �����ϰ� �ʹ�.
		target->ReceiveDamage();
		me->Destroy();
		// ��� �ð� �ʱ�ȭ
		currentTime = 0;
	}

	// ��ǥ: Ÿ���� ���� ������ ����� ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
	// 1. Ÿ����� �Ÿ��� �ʿ��ϴ�.
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	// 2. Ÿ����� �Ÿ��� ���� ������ ������ϱ�
	if (distance > attackRange)
	{
		// 3. ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
		mState = EEnemy5State::Idle;

		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UEnemy_5_FSM::UpdaetAttackDelay()
{

}

void UEnemy_5_FSM::DamageState()
{
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (currentTime > damageDelayTime)
	{
		// 3. ��� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemy5State::Idle;
		// ��� �ð� �ʱ�ȭ 
		currentTime = 0;

	}
}

void UEnemy_5_FSM::DieState()
{

	// ��� �Ʒ��� �������� �ʹ�.
	// ��ӿ ���� P = P0 + vt
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	// 1. ���� 2���� �̻� �����Դٸ�
	if (P.Z < -200.0f)
	{
		// 2. ���Ž�Ų��.
		me->Destroy();
	}
}

void UEnemy_5_FSM::OnDamageProcess(int32 damage)
{
	// ü�� ����
	hp--;
	me->dam = true;
	showdamage = damage;
	// ���� ü���� �����ִٸ�
	if (hp > 0)
	{
		// ���¸� �ǰ����� ��ȯ
		mState = EEnemy5State::Damage;

		currentTime = 0;
	}
	// �׷��� �ʴٸ�
	else
	{
		// ���¸� �������� ��ȯ
		mState = EEnemy5State::Die;
		// ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		me->Destroy();
	}


	ai->StopMovement();
}

// ���� ��ġ ��������
bool UEnemy_5_FSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector&
	dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

