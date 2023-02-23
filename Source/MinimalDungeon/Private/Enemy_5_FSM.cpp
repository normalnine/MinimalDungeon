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
	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;
	//UE_LOG(LogTemp, Warning, TEXT("idle"));
	// 2. 만약 경과 시간이 대기 시간을 초과했다면
	if (currentTime > idleDelayTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("change move"));
		// 3. 이동 상태로 전환하고 싶다.
		mState = EEnemy5State::Move;
		// 경과 시간 초기화 
		currentTime = 0;


		// 최초 랜덤한 위치 정해주기
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UEnemy_5_FSM::MoveState()
{
	//UE_LOG(LogTemp, Warning, TEXT("start move"));
	// 1. 타깃 목적지가 필요하다.
	FVector destination = target->GetActorLocation();
	// 2. 방향이 필요하다.
	FVector dir = destination - me->GetActorLocation();
	// 3. 방향으로 이동하고 싶다.
	//me->AddMovementInput(dir.GetSafeNormal());
	// NavigationSystem 객체 얻어오기
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	// 목적지 길 찾기 경로 데이터 검색
	FPathFindingQuery query;
	FAIMoveRequest req;
	// 목적지에서 인지할 수 있는 범위
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	// 길 찾기를 위한 쿼리 생성
	ai->BuildPathfindingQuery(req, query);
	// 길 찾기 결과 가져오기
	FPathFindingResult r = ns->FindPathSync(query);

	// 목적지 까지의 길찾기 성공 여부 확인
	if (r.Result == ENavigationQueryResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("moveto target"));
		// 타깃쪽으로 이동
		ai->MoveToLocation(destination);
	}
	else
	{
		// 랜덤 위치로 이동
		//UE_LOG(LogTemp, Warning, TEXT("moveto randompos"));
		auto result = ai->MoveToLocation(randomPos);
		// 목적지에 도착하면
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// 새로운 랜덤 위치 가져오기
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}

	// 타깃과 가까워지면 공격 상태로 전환하고 싶다.
	// 1. 만약 거리가 공격 범위 안에 들어오면
	if (dir.Size() < attackRange)
	{
		// 길 찾기 기능 정지
		ai->StopMovement();
	//	UE_LOG(LogTemp, Warning, TEXT("change attack"));
		// 2. 공격 상태로 전환하고 싶다.
		mState = EEnemy5State::Attack;

		// 공격 상태 전환 시 대기 시간이 바로 끝나도록 처리 
		currentTime = attackDelayTime;
	}
}

void UEnemy_5_FSM::AttackState()
{
	//UE_LOG(LogTemp, Warning, TEXT("attack start"));
		// 1. 시간이 흘러야 한다.
		currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 공격 시간이 됐으니까
	if (currentTime > attackDelayTime)
	{
		// 3. 공격하고 싶다.
		target->ReceiveDamage();
		me->Destroy();
		// 경과 시간 초기화
		currentTime = 0;
	}

	// 목표: 타깃이 공격 범위를 벗어나면 상태를 이동으로 전환하고 싶다.
	// 1. 타깃과의 거리가 필요하다.
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	// 2. 타깃과의 거리가 공격 범위를 벗어났으니까
	if (distance > attackRange)
	{
		// 3. 상태를 이동으로 전환하고 싶다.
		mState = EEnemy5State::Idle;

		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UEnemy_5_FSM::UpdaetAttackDelay()
{

}

void UEnemy_5_FSM::DamageState()
{
	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 만약 경과 시간이 대기 시간을 초과했다면
	if (currentTime > damageDelayTime)
	{
		// 3. 대기 상태로 전환하고 싶다.
		mState = EEnemy5State::Idle;
		// 경과 시간 초기화 
		currentTime = 0;

	}
}

void UEnemy_5_FSM::DieState()
{

	// 계속 아래로 내려가고 싶다.
	// 등속운동 공식 P = P0 + vt
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	// 1. 만약 2미터 이상 내려왔다면
	if (P.Z < -200.0f)
	{
		// 2. 제거시킨다.
		me->Destroy();
	}
}

void UEnemy_5_FSM::OnDamageProcess(int32 damage)
{
	// 체력 감소
	hp--;
	me->dam = true;
	showdamage = damage;
	// 만약 체력이 남아있다면
	if (hp > 0)
	{
		// 상태를 피격으로 전환
		mState = EEnemy5State::Damage;

		currentTime = 0;
	}
	// 그렇지 않다면
	else
	{
		// 상태를 죽음으로 전환
		mState = EEnemy5State::Die;
		// 캡슐 충돌체 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		me->Destroy();
	}


	ai->StopMovement();
}

// 랜덤 위치 가져오기
bool UEnemy_5_FSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector&
	dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

