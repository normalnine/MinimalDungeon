// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_1_FSM.h"
#include "Coin.h"
#include "VR_Player.h"
#include "Enemy_1.h"
#include <Kismet/GameplayStatics.h>
#include "MinimalDungeon.h"
#include <Components/CapsuleComponent.h>
#include "Enemy_1_Anim.h"
#include <AIController.h>
#include <NavigationSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include <Engine/EngineTypes.h>
#include "GameFramework/Actor.h"
#include <Engine/World.h>

// Sets default values for this component's properties
UEnemy_1_FSM::UEnemy_1_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/LJW/Enemys/SimpleSkeleton/anim/AM_Enemy_1.AM_Enemy_1'"));
	if (tempMontage.Succeeded())
	{
		damageMontage = tempMontage.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/LJW/sound/skeleton/Demon_07.Demon_07'"));
	if (tempSound.Succeeded())
	{
		attackSound = tempSound.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> hittempSound(TEXT("/Script/Engine.SoundWave'/Game/LJW/sound/skeleton/Demon_20.Demon_20'"));
	if (hittempSound.Succeeded())
	{
		hitonSound = hittempSound.Object;
	}
}


// Called when the game starts
void UEnemy_1_FSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AVR_Player::StaticClass());

	target = Cast<AVR_Player>(actor);

	me = Cast<AEnemy_1>(GetOwner());

	anim = Cast<UEnemy_1_Anim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());

	originPos = me->GetActorLocation();

	mat = UMaterialInstanceDynamic::Create(me->GetMesh()->GetMaterial(0), this);

	me->GetMesh()->SetMaterial(0, mat);
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
	//시야에 들어오면 움직이기 시작
	if (IsTargetTrace())
	{
		ChangeState(EEnemyState::Move);
	}

	else
	{
		//idleDelayTime 이 지나면	
		if (IsWaitComplete(idleDelayTime))
		{
			//현재상태를 Move 로 한다.
			ChangeState(EEnemyState::Move);
		}
	}
}
void UEnemy_1_FSM::MoveState()
{
	// 시야에 들어왔는지 여부
	bool bTrace = IsTargetTrace();

	//타겟을 향하는 방향을 구하고
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//처음 위치, 나의 현재 위치의 거리
	float dist = FVector::Distance(originPos, me->GetActorLocation());


	//시야에 들어왔다면
	if (bTrace)
	{
		//UE_LOG(LogTemp, Warning, TEXT("trace"));
		//만약에 target - me 거리가 공격범위보다 작으면
		if (dir.Length() < attackRange)
		{
			//UE_LOG(LogTemp, Warning, TEXT("change attack"));
			//상태를 Attack 으로 변경
			ChangeState(EEnemyState::Attack);
		}
		//그렇지 않으면
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("movetotarget"));
			ai->MoveToLocation(target->GetActorLocation());
		}
	}
	//시야에 들어오지 않았다면
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("movetorandpos"));
		// 랜덤한 위치까지 도착한 후 Idle 상태로 전환
		MoveToPos(randPos);
	}
}

void UEnemy_1_FSM::AttackState()
{	
	FVector des = target->GetActorLocation();

	FVector dir = des - me->GetActorLocation();

	FRotator dirx = dir.Rotation();

	me->SetActorRotation(dirx);

	ChangeState(EEnemyState::AttackDelay);
}
void UEnemy_1_FSM::UpdaetAttackDelay()
{
	if (IsWaitComplete(attackDelayTime))
	{

		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();

		if (dist < attackRange)
		{

			ChangeState(EEnemyState::Attack);
		}
		else
		{

			ChangeState(EEnemyState::Idle);
		}
	}
}
void UEnemy_1_FSM::DamageState()
{
	//damageDelayTime 이 지나면
	if (IsWaitComplete(damageDelayTime))
	{
		//Move 상태
		ChangeState(EEnemyState::Idle);
	}
}
void UEnemy_1_FSM::DieState()
{

	//아직 죽음 애니메이션이 끝나지 않았다면
	//바닥내려가지 않도록 처리
	if (anim->bDieDone == false)
	{
		return;
	}

	//계속 아래로 내려가고 싶다.
	//동속ㅇ운동ㅇ 공식 피=피제+브이티
	FVector p0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;
	//2. 만약에 p.Z 가 -200 보다 작으면 파괴한다
	if (p.Z < -200)
	{
		me->Destroy();
	}
	//3. 그렇지 않으면 해당 위치로 셋팅한다
	else
	{
		me->SetActorLocation(p);
	}


}
void UEnemy_1_FSM::UpdateReturnPos()
{
	MoveToPos(originPos);
}
void UEnemy_1_FSM::OnDamageProcess(int32 damage)
{
	//체력감소
	hp -= damage;
	UGameplayStatics::PlaySound2D(GetWorld(), hitonSound);
	//me->dam = true;
	showdamage = damage;



	//체력이 남아있다면
	if (hp > 0)
	{
		//상태를 피격으로 전환
		mState = EEnemyState::Damage;
		currentTime = 0;
		//피격 애니메이션 재생
		FString sectionName = FString::Printf(TEXT("Damage0"));
		anim->PlayDamageAnim(FName(*sectionName));
		//색을 빨간색으로 변경

		mat->SetVectorParameterValue(TEXT("EmissiveColor"), FVector4(1, 0, 0, 1));
		mat->SetScalarParameterValue(TEXT("Glow"), 50.0f);

		GetWorld()->GetTimerManager().ClearTimer(colorHandle);
		GetWorld()->GetTimerManager().SetTimer(colorHandle, this, &UEnemy_1_FSM::ColorOff, 0.5f, false);

	
	}


	else
	{
		//상태를 죽음으로 전환
		mState = EEnemyState::Die;
		//캡슐 충돌체 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//죽음 애니메이션 재생
		anim->PlayDamageAnim(TEXT("Die"));


	}
	//애니메이션 상태 동기화
	anim->animState = mState;
	ai->StopMovement();
}
bool UEnemy_1_FSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

bool UEnemy_1_FSM::IsTargetTrace()
{


	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	FVector dirSize = dir;
	dir.Normalize();


	FVector na = me->GetActorLocation();
	float dotvalue = FVector::DotProduct(na, dir);

	//acos
	float angle = UKismetMathLibrary::DegAcos(dotvalue);

	//구한 값이 40보다 작고 적과 플레이어와의 거리가 지정한 거리보다 작으면
	if (angle < 50 && dirSize.Size() < traceRange)
	{


		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);
		bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, me->GetActorLocation(), target->GetActorLocation(), ECollisionChannel::ECC_EngineTraceChannel4, param);

		if (bhit)
		{
			if (hitInfo.GetActor()->GetName().Contains(TEXT("Player")))
			{
				return true;
			}

		}

	}

	return false;

}

bool UEnemy_1_FSM::IsWaitComplete(float delayTime)
{

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > delayTime)
	{

		currTime = 0;

		return true;
	}


	return false;
}

void UEnemy_1_FSM::ChangeState(EEnemyState state)
{
	//상태 변경 로그를 출력하자
	//UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	//if (enumPtr != nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
	//		*enumPtr->GetNameStringByIndex((int32)mState),
	//		*enumPtr->GetNameStringByIndex((int32)state));
	//}

	//현재 상태를 갱신
	mState = state;

	//anim 의 상태 갱신
	anim->animState = state;

	//현재 시간 초기화
	currTime = 0;

	//ai 의 움직임 멈추자
	ai->StopMovement();

	//상태에 따른 초기설정
	switch (mState)
	{
	case EEnemyState::Attack:
		UGameplayStatics::PlaySound2D(GetWorld(),attackSound);
		//currTime = attackDelayTime;
		break;
	case EEnemyState::Move:
	{
		//네비게이션 시스템 가져오자
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//랜덤한 위치를 얻오자
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemyState::Damage:
	{
		//1. 랜덤한 값을 뽑는다 (0, 1 중)
		int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 이란 문자열을 만든다.
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. 몽타주를 플레이한다.
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState::Die:

		//충돌안되게 설정
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


		//죽으면 코인 스폰
		FTransform dropPos = me->GetTransform();
		GetWorld()->SpawnActor<ACoin>(DropFactory, dropPos);

		//Die 몽타주 실행
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));

		break;
	}
}
void UEnemy_1_FSM::ReceiveDamage()
{
	//피를 줄이자
	hp--;
	//hp 가 0보다 크면 Damage 상태로 전환
	if (hp > 0)
	{
		ChangeState(EEnemyState::Damage);
	}
	//그렇지 않으면 Die 상태로 전환
	else
	{

		ChangeState(EEnemyState::Die);
	}
}

void UEnemy_1_FSM::MoveToPos(FVector pos)
{
	//해당 위치(pos) 로 간다
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//만약에 목적지에 도착했다면
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle 상태로 전환
		ChangeState(EEnemyState::Idle);
	}
}
void UEnemy_1_FSM::ColorOff()
{

	mat->SetVectorParameterValue(TEXT("EmissiveColor"), FVector4(1, 1, 1, 1));
	mat->SetScalarParameterValue(TEXT("Glow"), 0);
}
