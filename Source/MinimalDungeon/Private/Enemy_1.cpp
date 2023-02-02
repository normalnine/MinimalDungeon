// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_1.h"
#include "Enemy_1_FSM.h"

// Sets default values
AEnemy_1::AEnemy_1()
{

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LJW/Enemys/EnemyMesh/skeleton.skeleton'"));
	//������ �ε� �����ϸ�
	if (tempMesh.Succeeded())
	{
		//�������Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//�޽� ��ġ �� ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	fsm = CreateDefaultSubobject<UEnemy_1_FSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void AEnemy_1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy_1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

