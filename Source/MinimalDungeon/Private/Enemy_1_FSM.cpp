// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_1_FSM.h"
#include "VR_Player.h"
#include "Enemy_1.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UEnemy_1_FSM::UEnemy_1_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemy_1_FSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AVR_Player::StaticClass());

	target = Cast<AVR_Player>(actor);

	me = Cast<AEnemy_1>(GetOwner());


}


// Called every frame
void UEnemy_1_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	case EEnemyState::ReturnPos:
		UpdateReturnPos();
		break;
	}
}


void UEnemy_1_FSM::IdleState()
{
	//시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;
	//만약경과ㅣ간이 대기 시간을 초과했다면
	if (currentTime > idleDelayTime)
	{
		//이동 상태로 전환하고 싶다
		mState = EEnemyState::Move;
		//경과시간 초기화
		currentTime = 0;

	}
}
void UEnemy_1_FSM::MoveState()
{
	//타깃
	FVector destination = target->GetActorLocation();
	//방향
	FVector dir = destination - me->GetActorLocation();
	//이동
	me->AddMovementInput(dir.GetSafeNormal());
}

void UEnemy_1_FSM::AttackState()
{

}
void UEnemy_1_FSM::UpdaetAttackDelay()
{

}
void UEnemy_1_FSM::DamageState()
{

}
void UEnemy_1_FSM::DieState()
{

}
void UEnemy_1_FSM::UpdateReturnPos()
{

}
