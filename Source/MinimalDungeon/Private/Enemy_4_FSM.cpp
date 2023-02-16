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
//������
void UEnemy_4_FSM::IdleState()
{

	//// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	//// 2. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (currentTime > 1)
	{
		// 3. �̵� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemy4State::Move;
		// ��� �ð� �ʱ�ȭ 
		currentTime = 0;


		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
	//	// ���� ������ ��ġ �����ֱ�
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
		
}
//�̵�����
void UEnemy_4_FSM::MoveState()
{
	UE_LOG(LogTemp, Warning, TEXT("Move"));
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	// ������ �� ã�� ��� ������ �˻�
	FPathFindingQuery query;
	FAIMoveRequest req;
	// ���������� ������ �� �ִ� ����
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(target->GetActorLocation());
	// �� ã�⸦ ���� ���� ����
	ai->BuildPathfindingQuery(req, query);
	// �� ã�� ��� ��������
	FPathFindingResult r = ns->FindPathSync(query);
	// ������ ������ ��ã�� ���� ���� Ȯ��
	if (r.Result == ENavigationQueryResult::Success)
	{
		// Ÿ�������� �̵�
		ai->MoveToLocation(target->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("Movetotarget"));
	}
	else
	{
		// ���� ��ġ�� �̵�
		auto result = ai->MoveToLocation(randomPos);
		UE_LOG(LogTemp, Warning, TEXT("Movetorandom"));
		// �������� �����ϸ�
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// ���ο� ���� ��ġ ��������
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	if (dir.Size() < attackRange)
	{
		ai->StopMovement();
		UE_LOG(LogTemp, Warning, TEXT("change to attack"));
		//���¸� Attack ���� ����
		EEnemy4State::Attack;
		anim->animState = mState;
	}

}
}
//���ݻ���
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
//���� ���
void UEnemy_4_FSM::UpdaetAttackDelay()
{

}
//�ǰݻ���
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
//��������
void UEnemy_4_FSM::DieState()
{

}
//����
void UEnemy_4_FSM::UpdateReturnPos()
{

}
void UEnemy_4_FSM::OnDamageProcess(int32 damage)
{
	hp -= damage;
	//ü���� �����ִٸ�
	if (hp > 0)
	{
		//���¸� �ǰ����� ��ȯ
		mState = EEnemy4State::Damage;
		currentTime = 0;
		//�ǰ� �ִϸ��̼� ���
		FString sectionName = FString::Printf(TEXT("Damage0"));
		anim->PlayDamageAnim(FName(*sectionName));
		
		//���� ���������� ����

	//	mat->SetVectorParameterValue(TEXT("EmissiveColor"), FVector4(1, 0, 0, 1));
		//mat->SetScalarParameterValue(TEXT("Glow"), 50.0f);

		//GetWorld()->GetTimerManager().ClearTimer(colorHandle);
		//GetWorld()->GetTimerManager().SetTimer(colorHandle, this, &UEnemy_3_FSM::ColorOff, 0.5f, false);

	}


	else
	{
		//���¸� �������� ��ȯ
		mState = EEnemy4State::Die;
		//ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		me->Destroy();
		//���� �ִϸ��̼� ���
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