// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_3_FSM.h"
#include "VR_Player.h"
#include "Enemy_3.h"
#include <Kismet/GameplayStatics.h>
#include "MinimalDungeon.h"
#include "Enemy_3_Anim.h"
#include <AIController.h>
#include <NavigationSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include <Engine/EngineTypes.h>
#include <Components/CapsuleComponent.h>
#include "GameFramework/Actor.h"
#include <Engine/World.h>
#include "Coin.h"


// Sets default values for this component's properties
UEnemy_3_FSM::UEnemy_3_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/LJW/Enemys/slime_TurtleShell/RPG_Monster_Duo_PBR_Polyart/Animations/Slime/AM_Enemy_3.AM_Enemy_3'"));
	if (tempMontage.Succeeded())
	{
		damageMontage = tempMontage.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/LJW/sound/slime/Splat_01.Splat_01'"));
	if (tempSound.Succeeded())
	{
		attackSound = tempSound.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> hittempSound(TEXT("/Script/Engine.SoundWave'/Game/LJW/sound/slime/Splat_21.Splat_21'"));
	if (hittempSound.Succeeded())
	{
		hitonSound = hittempSound.Object;
	}
}


// Called when the game starts
void UEnemy_3_FSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AVR_Player::StaticClass());

	target = Cast<AVR_Player>(actor);

	me = Cast<AEnemy_3>(GetOwner());

	anim = Cast<UEnemy_3_Anim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());

	originPos = me->GetActorLocation();

	mat = UMaterialInstanceDynamic::Create(me->GetMesh()->GetMaterial(0), this);

	me->GetMesh()->SetMaterial(0, mat);

}


// Called every frame
void UEnemy_3_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemy3State::Idle:
		IdleState();
		break;
	case EEnemy3State::Move:
		MoveState();
		break;
	case EEnemy3State::Attack:
		AttackState();
		break;
	case EEnemy3State::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemy3State::Damage:
		DamageState();
		break;
	case EEnemy3State::Die:
		DieState();
		break;
	}
}

void UEnemy_3_FSM::IdleState()
{
	//시야에 들어오면 움직이기 시작
	if (IsTargetTrace())
	{
		ChangeState(EEnemy3State::Move);
	}

	else
	{
			
		if (IsWaitComplete(idleDelayTime))
		{
			
			ChangeState(EEnemy3State::Move);
		}
	}
}
void UEnemy_3_FSM::MoveState()
{
	
	bool bTrace = IsTargetTrace();

	//방향
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//거리
	float dist = FVector::Distance(originPos, me->GetActorLocation());


	
	if (dist > moveRange)
	{
		MoveToPos(randPos);
	}
	//시야에 들어왔다면
	else if (bTrace)
	{
	

		//target - me 거리가 공격범위보다 작으면
		if (dir.Length() < attackRange)
		{

			//상태를 Attack 으로 변경
			ChangeState(EEnemy3State::Attack);
		}
		//그렇지 않으면
		else
		{
			ai->MoveToLocation(target->GetActorLocation());
		}
	}
	//시야에 들어오지 않았다면
	else
	{
	
		MoveToPos(randPos);
	}
}

void UEnemy_3_FSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	FVector des = target->GetActorLocation();

	FVector dir = des - me->GetActorLocation();

	FRotator dirx = dir.Rotation();

	me->SetActorRotation(dirx);


	if (currentTime > 2)
	{

		ChangeState(EEnemy3State::AttackDelay);

		currentTime = 0;
	}

}
void UEnemy_3_FSM::UpdaetAttackDelay()
{
	if (IsWaitComplete(attackDelayTime))
	{

		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();

		if (dist < attackRange)
		{

			ChangeState(EEnemy3State::Attack);
		}
		else
		{
			ChangeState(EEnemy3State::Idle);
		}
	}
}
void UEnemy_3_FSM::DamageState()
{
	
	if (IsWaitComplete(damageDelayTime))
	{
		//Move 상태
		ChangeState(EEnemy3State::Idle);
	}
}
void UEnemy_3_FSM::DieState()
{

	////아직 죽음 애니메이션이 끝나지 않았다면
	////바닥내려가지 않도록 처리
	//if (anim->bDieDone == false)
	//{
	//	return;
	//}

	////계속 아래로 내려가고 싶다.
	////동속ㅇ운동ㅇ 공식 피=피제+브이티
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

void UEnemy_3_FSM::OnDamageProcess(int32 damage)
{
	
	hp -= damage;
	UGameplayStatics::PlaySound2D(GetWorld(), hitonSound);
	dam = true;
//	me->dam = true;
	showdamage = damage;

	if (hp > 0)
	{
		//상태를 피격으로 전환
		mState = EEnemy3State::Damage;
		currentTime = 0;
		//피격 애니메이션 재생
		FString sectionName = FString::Printf(TEXT("Damage0"));
		anim->PlayDamageAnim(FName(*sectionName));

		//색을 빨간색으로 변경
		mat->SetVectorParameterValue(TEXT("EmissiveColor"), FVector4(1, 0, 0, 1));
		mat->SetScalarParameterValue(TEXT("Glow"), 50.0f);

		GetWorld()->GetTimerManager().ClearTimer(colorHandle);
		GetWorld()->GetTimerManager().SetTimer(colorHandle, this, &UEnemy_3_FSM::ColorOff, 0.5f, false);

		if(!me->hiton)
		{
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<AActor>(EnemyFactory, me->GetActorLocation() + me->GetActorRightVector()*200, me->GetActorRotation(), params);
			me->hiton = true;
		}
	}


	else
	{
		//상태를 죽음으로 전환
		mState = EEnemy3State::Die;
		//캡슐 충돌체 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//죽음 애니메이션 재생
		anim->PlayDamageAnim(TEXT("Die"));
		//죽으면 코인 스폰
		FTransform dropPos = me->GetTransform();
		GetWorld()->SpawnActor<AActor>(DropFactory, dropPos);
		//시간지나면 디스트로이하기
		FTimerHandle ddd;
		GetWorld()->GetTimerManager().SetTimer(ddd, this, &UEnemy_3_FSM::destroyme, 4.0f, false);

	}
	//애니메이션 상태 동기화
	anim->animState = mState;
	//ai->StopMovement();
}
bool UEnemy_3_FSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	//랜덤위치 지정
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

bool UEnemy_3_FSM::IsTargetTrace()
{


	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	dir.Normalize();


	FVector na = me->GetActorLocation();
	float dotvalue = FVector::DotProduct(na, dir);

	
	float angle = UKismetMathLibrary::DegAcos(dotvalue);

	//30보다 작고 적과 플레이어와의 거리가 지정한 거리보다 작으면
	if (angle < 30 && dir.Length() < traceRange)
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

bool UEnemy_3_FSM::IsWaitComplete(float delayTime)
{

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > delayTime)
	{

		currTime = 0;

		return true;
	}


	return true;
}

void UEnemy_3_FSM::ChangeState(EEnemy3State state)
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

	
	//ai->StopMovement();

	//상태에 따른 초기설정
	switch (mState)
	{
	case EEnemy3State::Attack:
		UGameplayStatics::PlaySound2D(GetWorld(), attackSound);
		currTime = attackDelayTime;
		break;
	case EEnemy3State::Move:
	{
		//네비게이션 시스템 가져오자
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//랜덤한 위치를 얻오자
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemy3State::Damage:
	{
		
		int32 rand = 0;	
		
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemy3State::Die:

		//충돌안되게 설정
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Die 몽타주 실행
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));
		
	
		break;
	}
}

void UEnemy_3_FSM::MoveToPos(FVector pos)
{
	//해당 위치(pos) 로 간다
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//만약에 목적지에 도착했다면
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle 상태로 전환
		ChangeState(EEnemy3State::Idle);
	}
}
void UEnemy_3_FSM::ColorOff()
{

	mat->SetVectorParameterValue(TEXT("EmissiveColor"), FVector4(1, 1, 1, 1));
	mat->SetScalarParameterValue(TEXT("Glow"), 0);
}
void UEnemy_3_FSM::destroyme()
{
	me->Destroy();
}
