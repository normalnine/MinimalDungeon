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
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "Coin.h"

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

	mat = UMaterialInstanceDynamic::Create(me->GetMesh()->GetMaterial(0), this);

	me->GetMesh()->SetMaterial(0, mat);
	
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
	case EEnemy4State::sleep:
		isSleep();
		break;
	}
}
//대기상태
void UEnemy_4_FSM::IdleState()
{

	
	//시야에 들어오면 움직이기 시작
	if (IsTargetTrace())
	{
		ChangeState(EEnemy4State::Move);
	}

	else
	{
		//idleDelayTime 이 지나면	
		if (IsWaitComplete(idleDelayTime))
		{
			//현재상태를 Move 로 한다.
			ChangeState(EEnemy4State::Move);
		}
	}
		
}
//이동상태
void UEnemy_4_FSM::MoveState()
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
		UE_LOG(LogTemp, Warning, TEXT("trace"));
		//만약에 target - me 거리가 공격범위보다 작으면
		if (dir.Length() < attackRange)
		{
			UE_LOG(LogTemp, Warning, TEXT("change attack"));
			//상태를 Attack 으로 변경
			ChangeState(EEnemy4State::Attack);
		}
		//그렇지 않으면
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("movetotarget"));
			ai->MoveToLocation(target->GetActorLocation());
		}
	}
	//시야에 들어오지 않았다면
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("movetorandpos"));
		// 랜덤한 위치까지 도착한 후 Idle 상태로 전환
		MoveToPos(randPos);
	}
}
//공격상태
void UEnemy_4_FSM::AttackState()
{
	ChangeState(EEnemy4State::AttackDelay);
	
}

//공격 대기
void UEnemy_4_FSM::UpdaetAttackDelay()
{
	if (IsWaitComplete(attackDelayTime))
	{

		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();

		if (dist < attackRange)
		{

			ChangeState(EEnemy4State::Attack);
		}
		else
		{

			ChangeState(EEnemy4State::Idle);
		}
	}
}
//피격상태
void UEnemy_4_FSM::DamageState()
{
	//damageDelayTime 이 지나면
	if (IsWaitComplete(damageDelayTime))
	{
		//Move 상태
		ChangeState(EEnemy4State::Idle);
	}
}
//죽음상태
void UEnemy_4_FSM::DieState()
{
	//아직 죽음 애니메이션이 끝나지 않았다면
		//바닥내려가지 않도록 처리
	/*if (anim->bDieDone == false)
	{
		return;
	}*/

	//계속 아래로 내려가고 싶다.
	//동속ㅇ운동ㅇ 공식 피=피제+브이티
	//FVector p0 = me->GetActorLocation();
	//FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	//FVector p = p0 + vt;
	////2. 만약에 p.Z 가 -200 보다 작으면 파괴한다
	//if (p.Z < -200)
	//{
	//	me->Destroy();
	//}
	////3. 그렇지 않으면 해당 위치로 셋팅한다
	//else
	//{
	//	me->SetActorLocation(p);
	//}
}
//리턴
void UEnemy_4_FSM::UpdateReturnPos()
{

}

void UEnemy_4_FSM::isSleep()
{
	UE_LOG(LogTemp, Warning, TEXT("sleep"));
	anim->animState = mState;

}

void UEnemy_4_FSM::OnDamageProcess(int32 damage)
{
	hp -= damage;
	dam = true;
	
	if (hp > 0)
	{
		ChangeState(EEnemy4State::Damage);

		//피격되면 트류
		anim->hitme = true;

		//색을 빨간색으로 변경

		mat->SetVectorParameterValue(TEXT("EmissiveColor"), FVector4(1, 0, 0, 1));
		mat->SetScalarParameterValue(TEXT("Glow"), 50.0f);

		GetWorld()->GetTimerManager().ClearTimer(colorHandle);
		GetWorld()->GetTimerManager().SetTimer(colorHandle, this, &UEnemy_4_FSM::ColorOff, 0.5f, false);

		//죽으면 코인 스폰
		FTransform dropPos = me->GetTransform();
		GetWorld()->SpawnActor<ACoin>(DropFactory, dropPos);
	}


	else
	{
		//상태를 죽음으로 전환
		mState = EEnemy4State::Die;
		//캡슐 충돌체 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
		//죽음 애니메이션 재생
		anim->PlayDamageAnim(TEXT("Die"));
		FTimerHandle ddd;
		GetWorld()->GetTimerManager().SetTimer(ddd, this, &UEnemy_4_FSM::destroyme, 4.0f, false);

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
bool UEnemy_4_FSM::IsTargetTrace()
{


	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	FVector dirSize = dir;
	dir.Normalize();


	FVector na = me->GetActorLocation();
	float dotvalue = FVector::DotProduct(na, dir);

	//acos
	float angle = UKismetMathLibrary::DegAcos(dotvalue);

	//구한 값이 40보다 작고 적과 플레이어와의 거리가 지정한 거리보다 작으면
	if (angle < 40 && dirSize.Size() < traceRange)
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
bool UEnemy_4_FSM::IsWaitComplete(float delayTime)
{

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > delayTime)
	{

		currTime = 0;

		return true;
	}


	return false;
}

void UEnemy_4_FSM::ChangeState(EEnemy4State state)
{
	//상태 변경 로그를 출력하자
	//UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemy4State"), true);
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
	currentTime = 0;

	//ai 의 움직임 멈추자
	if(ai != nullptr)
	{
	ai->StopMovement();
	}

	//상태에 따른 초기설정
	switch (mState)
	{
	case EEnemy4State::Attack:
		//currTime = attackDelayTime;
		break;
	case EEnemy4State::Move:
	{
		//네비게이션 시스템 가져오자
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//랜덤한 위치를 얻오자
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemy4State::Damage:
	{
//		//1. 랜덤한 값을 뽑는다 (0, 1 중)
//		int32 rand = FMath::RandRange(0, 1);
//		//2. Damage0, Damage1 이란 문자열을 만든다.
//		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
//		//3. 몽타주를 플레이한다.
//		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));

	FString sectionName = FString::Printf(TEXT("Damage0"));
	anim->PlayDamageAnim(FName(*sectionName));
	}
	break;
	case EEnemy4State::Die:

		//충돌안되게 설정
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//Die 몽타주 실행
//		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));

		break;
	}
}
void UEnemy_4_FSM::MoveToPos(FVector pos)
{
	//해당 위치(pos) 로 간다
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//만약에 목적지에 도착했다면
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle 상태로 전환
		ChangeState(EEnemy4State::Idle);
	}
}

void UEnemy_4_FSM::ColorOff()
{
	mat->SetVectorParameterValue(TEXT("EmissiveColor"), FVector4(1, 1, 1, 1));
	mat->SetScalarParameterValue(TEXT("Glow"), 0);
}
void UEnemy_4_FSM::destroyme()
{
	me->Destroy();
}
