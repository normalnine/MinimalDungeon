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
	//�þ߿� ������ �����̱� ����
	if (IsTargetTrace())
	{
		ChangeState(EEnemyState::Move);
	}

	else
	{
		//idleDelayTime �� ������	
		if (IsWaitComplete(idleDelayTime))
		{
			//������¸� Move �� �Ѵ�.
			ChangeState(EEnemyState::Move);
		}
	}
}
void UEnemy_1_FSM::MoveState()
{
	// �þ߿� ���Դ��� ����
	bool bTrace = IsTargetTrace();

	//Ÿ���� ���ϴ� ������ ���ϰ�
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//ó�� ��ġ, ���� ���� ��ġ�� �Ÿ�
	float dist = FVector::Distance(originPos, me->GetActorLocation());


	//������ �� �ִ� �ݰ��� �Ѿ�ٸ�
	if (dist > moveRange)
	{
		//���¸� ReturnPos �� ����
		ChangeState(EEnemyState::ReturnPos);
	}
	//�þ߿� ���Դٸ�
	else if (bTrace)
	{
		/*wahng++;
		UE_LOG(LogTemp, Warning, TEXT("%d"), wahng);*/

		//���࿡ target - me �Ÿ��� ���ݹ������� ������
		if (dir.Length() < attackRange)
		{

			//���¸� Attack ���� ����
			ChangeState(EEnemyState::Attack);
		}
		//�׷��� ������
		else
		{
			ai->MoveToLocation(target->GetActorLocation());
		}
	}
	//�þ߿� ������ �ʾҴٸ�
	else
	{
		// ������ ��ġ���� ������ �� Idle ���·� ��ȯ
		MoveToPos(randPos);
	}
}

void UEnemy_1_FSM::AttackState()
{
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
	//damageDelayTime �� ������
	if (IsWaitComplete(damageDelayTime))
	{
		//Move ����
		ChangeState(EEnemyState::Idle);
	}
}
void UEnemy_1_FSM::DieState()
{

	//���� ���� �ִϸ��̼��� ������ �ʾҴٸ�
	//�ٴڳ������� �ʵ��� ó��
	if (anim->bDieDone == false)
	{
		return;
	}

	//��� �Ʒ��� �������� �ʹ�.
	//���Ӥ���� ���� ��=����+����Ƽ
	FVector p0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;
	//2. ���࿡ p.Z �� -200 ���� ������ �ı��Ѵ�
	if (p.Z < -200)
	{
		me->Destroy();
	}
	//3. �׷��� ������ �ش� ��ġ�� �����Ѵ�
	else
	{
		me->SetActorLocation(p);
	}


}
void UEnemy_1_FSM::UpdateReturnPos()
{
	MoveToPos(originPos);
}
void UEnemy_1_FSM::OnDamageProcess()
{
	//ü�°���
	hp--;
	//ü���� �����ִٸ�
	if (hp > 0)
	{
		//���¸� �ǰ����� ��ȯ
		mState = EEnemyState::Damage;
		currentTime = 0;
		//�ǰ� �ִϸ��̼� ���
		
		FString sectionName = FString::Printf(TEXT("Damage0"));
		anim->PlayDamageAnim(FName(*sectionName));
		
	}
	else 
	{
		//���¸� �������� ��ȯ
		mState = EEnemyState::Die;
		//ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//���� �ִϸ��̼� ���
		anim->PlayDamageAnim(TEXT("Die"));
		

	}
	//�ִϸ��̼� ���� ����ȭ
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
	dir.Normalize();


	FVector na = me->GetActorLocation();
	float dotvalue = FVector::DotProduct(na, dir);

	//2������ ���� ���� acos
	float angle = UKismetMathLibrary::DegAcos(dotvalue);

	//3������ ���� ���� 40���� �۰� ���� �÷��̾���� �Ÿ��� ������ �Ÿ����� ������
	if (dotvalue < 40 && dir.Length() < traceRange)
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
	//���� ���� �α׸� �������
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
			*enumPtr->GetNameStringByIndex((int32)mState),
			*enumPtr->GetNameStringByIndex((int32)state));
	}

	//���� ���¸� ����
	mState = state;

	//anim �� ���� ����
	anim->animState = state;

	//���� �ð� �ʱ�ȭ
	currTime = 0;

	//ai �� ������ ������
	ai->StopMovement();

	//���¿� ���� �ʱ⼳��
	switch (mState)
	{
	case EEnemyState::Attack:
		//currTime = attackDelayTime;
		break;
	case EEnemyState::Move:
	{
		//�׺���̼� �ý��� ��������
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//������ ��ġ�� �����
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemyState::Damage:
	{
		//1. ������ ���� �̴´� (0, 1 ��)
		int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 �̶� ���ڿ��� �����.
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. ��Ÿ�ָ� �÷����Ѵ�.
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState::Die:

		//�浹�ȵǰ� ����
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Die ��Ÿ�� ����
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));
		FTransform dropPos = me->GetTransform();
		GetWorld()->SpawnActor<ACoin>(DropFactory, dropPos);
		break;
	}
}
void UEnemy_1_FSM::ReceiveDamage()
{
	//�Ǹ� ������
	hp--;
	//hp �� 0���� ũ�� Damage ���·� ��ȯ
	if (hp > 0)
	{
		ChangeState(EEnemyState::Damage);
	}
	//�׷��� ������ Die ���·� ��ȯ
	else
	{

		ChangeState(EEnemyState::Die);
	}
}

void UEnemy_1_FSM::MoveToPos(FVector pos)
{
	//�ش� ��ġ(pos) �� ����
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//���࿡ �������� �����ߴٸ�
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle ���·� ��ȯ
		ChangeState(EEnemyState::Idle);
	}
}
