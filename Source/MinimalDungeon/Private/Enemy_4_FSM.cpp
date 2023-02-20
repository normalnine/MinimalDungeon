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
//������
void UEnemy_4_FSM::IdleState()
{

	
	//�þ߿� ������ �����̱� ����
	if (IsTargetTrace())
	{
		ChangeState(EEnemy4State::Move);
	}

	else
	{
		//idleDelayTime �� ������	
		if (IsWaitComplete(idleDelayTime))
		{
			//������¸� Move �� �Ѵ�.
			ChangeState(EEnemy4State::Move);
		}
	}
		
}
//�̵�����
void UEnemy_4_FSM::MoveState()
{
	// �þ߿� ���Դ��� ����
	bool bTrace = IsTargetTrace();

	//Ÿ���� ���ϴ� ������ ���ϰ�
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//ó�� ��ġ, ���� ���� ��ġ�� �Ÿ�
	float dist = FVector::Distance(originPos, me->GetActorLocation());


	//�þ߿� ���Դٸ�
	if (bTrace)
	{
		UE_LOG(LogTemp, Warning, TEXT("trace"));
		//���࿡ target - me �Ÿ��� ���ݹ������� ������
		if (dir.Length() < attackRange)
		{
			UE_LOG(LogTemp, Warning, TEXT("change attack"));
			//���¸� Attack ���� ����
			ChangeState(EEnemy4State::Attack);
		}
		//�׷��� ������
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("movetotarget"));
			ai->MoveToLocation(target->GetActorLocation());
		}
	}
	//�þ߿� ������ �ʾҴٸ�
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("movetorandpos"));
		// ������ ��ġ���� ������ �� Idle ���·� ��ȯ
		MoveToPos(randPos);
	}
}
//���ݻ���
void UEnemy_4_FSM::AttackState()
{
	ChangeState(EEnemy4State::AttackDelay);
	
}

//���� ���
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
//�ǰݻ���
void UEnemy_4_FSM::DamageState()
{
	//damageDelayTime �� ������
	if (IsWaitComplete(damageDelayTime))
	{
		//Move ����
		ChangeState(EEnemy4State::Idle);
	}
}
//��������
void UEnemy_4_FSM::DieState()
{
	//���� ���� �ִϸ��̼��� ������ �ʾҴٸ�
		//�ٴڳ������� �ʵ��� ó��
	/*if (anim->bDieDone == false)
	{
		return;
	}*/

	//��� �Ʒ��� �������� �ʹ�.
	//���Ӥ���� ���� ��=����+����Ƽ
	//FVector p0 = me->GetActorLocation();
	//FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	//FVector p = p0 + vt;
	////2. ���࿡ p.Z �� -200 ���� ������ �ı��Ѵ�
	//if (p.Z < -200)
	//{
	//	me->Destroy();
	//}
	////3. �׷��� ������ �ش� ��ġ�� �����Ѵ�
	//else
	//{
	//	me->SetActorLocation(p);
	//}
}
//����
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

		//�ǰݵǸ� Ʈ��
		anim->hitme = true;

		//���� ���������� ����

		mat->SetVectorParameterValue(TEXT("EmissiveColor"), FVector4(1, 0, 0, 1));
		mat->SetScalarParameterValue(TEXT("Glow"), 50.0f);

		GetWorld()->GetTimerManager().ClearTimer(colorHandle);
		GetWorld()->GetTimerManager().SetTimer(colorHandle, this, &UEnemy_4_FSM::ColorOff, 0.5f, false);

		//������ ���� ����
		FTransform dropPos = me->GetTransform();
		GetWorld()->SpawnActor<ACoin>(DropFactory, dropPos);
	}


	else
	{
		//���¸� �������� ��ȯ
		mState = EEnemy4State::Die;
		//ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
		//���� �ִϸ��̼� ���
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

	//���� ���� 40���� �۰� ���� �÷��̾���� �Ÿ��� ������ �Ÿ����� ������
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
	//���� ���� �α׸� �������
	//UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemy4State"), true);
	//if (enumPtr != nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
	//		*enumPtr->GetNameStringByIndex((int32)mState),
	//		*enumPtr->GetNameStringByIndex((int32)state));
	//}

	//���� ���¸� ����
	mState = state;

	//anim �� ���� ����
	anim->animState = state;

	//���� �ð� �ʱ�ȭ
	currTime = 0;
	currentTime = 0;

	//ai �� ������ ������
	if(ai != nullptr)
	{
	ai->StopMovement();
	}

	//���¿� ���� �ʱ⼳��
	switch (mState)
	{
	case EEnemy4State::Attack:
		//currTime = attackDelayTime;
		break;
	case EEnemy4State::Move:
	{
		//�׺���̼� �ý��� ��������
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//������ ��ġ�� �����
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemy4State::Damage:
	{
//		//1. ������ ���� �̴´� (0, 1 ��)
//		int32 rand = FMath::RandRange(0, 1);
//		//2. Damage0, Damage1 �̶� ���ڿ��� �����.
//		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
//		//3. ��Ÿ�ָ� �÷����Ѵ�.
//		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));

	FString sectionName = FString::Printf(TEXT("Damage0"));
	anim->PlayDamageAnim(FName(*sectionName));
	}
	break;
	case EEnemy4State::Die:

		//�浹�ȵǰ� ����
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//Die ��Ÿ�� ����
//		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));

		break;
	}
}
void UEnemy_4_FSM::MoveToPos(FVector pos)
{
	//�ش� ��ġ(pos) �� ����
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//���࿡ �������� �����ߴٸ�
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle ���·� ��ȯ
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
