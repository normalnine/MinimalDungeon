// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_2_FSM.h"
#include "VR_Player.h"
#include "Enemy_2_Bullet.h"
#include "Enemy_2.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include "Enemy_2_Anim.h"
#include "Coin.h"


// Sets default values for this component's properties
UEnemy_2_FSM::UEnemy_2_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemy_2_FSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),AVR_Player::StaticClass());

	target = Cast<AVR_Player>(actor);

	me = Cast<AEnemy_2>(GetOwner());
	
	mat2 = UMaterialInstanceDynamic::Create(me->GetMesh()->GetMaterial(0), this);

	me->GetMesh()->SetMaterial(0, mat2);

	anim = Cast<UEnemy_2_Anim>(me->GetMesh()->GetAnimInstance());
}


// Called every frame
void UEnemy_2_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemy2State::Idle:
		IdleState();
		break;
	case EEnemy2State::Attack:
		AttackState();
		break;
	case EEnemy2State::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemy2State::Damage:
		DamageState();
		break;
	case EEnemy2State::Die:
		DieState();
		break;
	}
}
//대기상태
void UEnemy_2_FSM::IdleState()
{
	FVector des = target->GetActorLocation();

	FVector dir = des-me->GetActorLocation();

	if (dir.Size() <attackRange)
	{
		currentTime += GetWorld()->DeltaTimeSeconds;

		if (currentTime > DelayTime)
		{
			mState = EEnemy2State::Attack;
			currentTime = 0;
			anim->animState = mState;
		}
		
	}
	anim->animState = mState;
}
//공격상태
void UEnemy_2_FSM::AttackState()
{
	FVector des = target->GetActorLocation();

	FVector dir = des - me->GetActorLocation();

	FRotator dirx = dir.Rotation();
	//플레이어 방향으로
	//me->GetMesh()->GetSocketRotation(TEXT("Socket")) = dirx;

	//총알을  소켓 에서 스폰한다
	GetWorld()->SpawnActor<AEnemy_2_Bullet>(bulletFactory,me->GetMesh()->GetSocketLocation(TEXT("Socket")),dirx);

	//대기로 변경
	mState = EEnemy2State::AttackDelay;

	
}
//공격 대기
void UEnemy_2_FSM::UpdaetAttackDelay()
{
	//2초 대기
	/*FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UEnemy_2_FSM::IdleState, 5.0f, true);*/
	
	
		currentTime += GetWorld()->DeltaTimeSeconds;

		if (currentTime > DelayTime)
		{
			mState = EEnemy2State::Idle;
			currentTime = 0;
		}

	
	
}
//피격상태
void UEnemy_2_FSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > DelayTime)
	{
		mState = EEnemy2State::Idle;
		currentTime = 0;
	}
}
//죽음상태
void UEnemy_2_FSM::DieState()
{
	FVector p0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;
	me->SetActorLocation(p);
	//2. 만약에 p.Z 가 -200 보다 작으면 파괴한다
	if (p.Z < -200)
	{
		me->Destroy();
	}
}
void UEnemy_2_FSM::OnDamageProcess(int32 damage)
{

	//체력감소
	hp -= damage;
	
	//체력이 남아있다면
	if (hp > 0)
	{
		//상태를 피격으로 전환
		mState = EEnemy2State::Damage;

		FString sectionName = FString::Printf(TEXT("Damage0"));
		anim->PlayDamageAnim(FName(*sectionName));


		//currentTime = 0;
		mat2->SetVectorParameterValue(TEXT("EmissiveColor"), FVector4(1, 0, 0, 1));
		mat2->SetScalarParameterValue(TEXT("Glow"), 50.0f);

		GetWorld()->GetTimerManager().ClearTimer(colorHandle);
		GetWorld()->GetTimerManager().SetTimer(colorHandle, this, &UEnemy_2_FSM::ColorOff, 0.5f, false);
		
	}
	//그렇지않다면
	else {
		//상태를 죽음으로 전환
		mState = EEnemy2State::Die;
		//캡슐 충돌체 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//죽으면 코인 스폰
		FTransform dropPos = me->GetTransform();
		GetWorld()->SpawnActor<ACoin>(DropFactory, dropPos);

	}
	
}
void UEnemy_2_FSM::ColorOff()
{

	mat2->SetVectorParameterValue(TEXT("EmissiveColor"), FVector4(1, 1, 1, 1));
	mat2->SetScalarParameterValue(TEXT("Glow"), 0);
}