// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_4_FSM.h"
#include "VR_Player.h"
#include "Enemy_4.h"
#include <Kismet/GameplayStatics.h>
#include "MinimalDungeon.h"
#include <Components/CapsuleComponent.h>
#include "Enemy_4_Anim.h"
#include <AIController.h>
#include <NavigationSystem.h>

// Sets default values for this component's properties
UEnemy_4_FSM::UEnemy_4_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemy_4_FSM::BeginPlay()
{
	Super::BeginPlay();


	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AVR_Player::StaticClass());

	target = Cast<AVR_Player>(actor);

	me = Cast<AEnemy_4>(GetOwner());

	anim = Cast<UEnemy_4_Anim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());
	
}


// Called every frame
void UEnemy_4_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemy4State::Idle:
		IdleState();
		break;
	case EEnemy4State::Move:
		MoveState();
		break;
	case EEnemy4State::Attack:
		AttackState();
		break;
	case EEnemy4State::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemy4State::Damage:
		DamageState();
		break;
	case EEnemy4State::Die:
		DieState();
		break;
	case EEnemy4State::ReturnPos:
		UpdateReturnPos();
		break;
	}
}
//대기상태
void UEnemy_4_FSM::IdleState()
{

	//// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;
	//// 2. 만약 경과 시간이 대기 시간을 초과했다면
	if (currentTime > 1)
	{
		// 3. 이동 상태로 전환하고 싶다.
		mState = EEnemy4State::Move;
		// 경과 시간 초기화 
		currentTime = 0;


		// 애니메이션 상태 동기화
		anim->animState = mState;
	//	// 최초 랜덤한 위치 정해주기
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
		
}
//이동상태
void UEnemy_4_FSM::MoveState()
{
	UE_LOG(LogTemp, Warning, TEXT("Move"));
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	// 목적지 길 찾기 경로 데이터 검색
	FPathFindingQuery query;
	FAIMoveRequest req;
	// 목적지에서 인지할 수 있는 범위
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(target->GetActorLocation());
	// 길 찾기를 위한 쿼리 생성
	ai->BuildPathfindingQuery(req, query);
	// 길 찾기 결과 가져오기
	FPathFindingResult r = ns->FindPathSync(query);
	// 목적지 까지의 길찾기 성공 여부 확인
	if (r.Result == ENavigationQueryResult::Success)
	{
		// 타깃쪽으로 이동
		ai->MoveToLocation(target->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("Movetotarget"));
	}
	else
	{
		// 랜덤 위치로 이동
		auto result = ai->MoveToLocation(randomPos);
		UE_LOG(LogTemp, Warning, TEXT("Movetorandom"));
		// 목적지에 도착하면
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// 새로운 랜덤 위치 가져오기
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	if (dir.Size() < attackRange)
	{
		ai->StopMovement();
		UE_LOG(LogTemp, Warning, TEXT("change to attack"));
		//상태를 Attack 으로 변경
		EEnemy4State::Attack;
		anim->animState = mState;
	}

}
}
//공격상태
void UEnemy_4_FSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	UE_LOG(LogTemp, Warning, TEXT("attack"));
	if (currentTime > attackDelayTime)
	{
		
		currentTime = 0;
		EEnemy4State::AttackDelay;
	}
	float dis = FVector::Distance(target->GetActorLocation(),me->GetActorLocation());
	if (dis>attackRange)
	{
		mState = EEnemy4State::Move;
		anim->animState = mState;
	}

	
}
//공격 대기
void UEnemy_4_FSM::UpdaetAttackDelay()
{

}
//피격상태
void UEnemy_4_FSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > damageDelayTime)
	{
		mState = EEnemy4State::Move;
		currentTime = 0;
		anim->animState = mState;
	}
}
//죽음상태
void UEnemy_4_FSM::DieState()
{

}
//리턴
void UEnemy_4_FSM::UpdateReturnPos()
{

}
void UEnemy_4_FSM::OnDamageProcess(int32 damage)
{
	hp -= damage;
	//체력이 남아있다면
	if (hp > 0)
	{
		//상태를 피격으로 전환
		mState = EEnemy4State::Damage;
		currentTime = 0;
		//피격 애니메이션 재생
		FString sectionName = FString::Printf(TEXT("Damage0"));
		anim->PlayDamageAnim(FName(*sectionName));
		
		//색을 빨간색으로 변경

	//	mat->SetVectorParameterValue(TEXT("EmissiveColor"), FVector4(1, 0, 0, 1));
		//mat->SetScalarParameterValue(TEXT("Glow"), 50.0f);

		//GetWorld()->GetTimerManager().ClearTimer(colorHandle);
		//GetWorld()->GetTimerManager().SetTimer(colorHandle, this, &UEnemy_3_FSM::ColorOff, 0.5f, false);

	}


	else
	{
		//상태를 죽음으로 전환
		mState = EEnemy4State::Die;
		//캡슐 충돌체 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		me->Destroy();
		//죽음 애니메이션 재생
		//anim->PlayDamageAnim(TEXT("Die"));

	}
	ai->StopMovement();
	anim->animState = mState;
}
bool UEnemy_4_FSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}