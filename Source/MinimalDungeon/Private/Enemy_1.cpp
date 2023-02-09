// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_1.h"
#include "Enemy_1_FSM.h"

// Sets default values
AEnemy_1::AEnemy_1()
{

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SKELETON.SKELETON'"));
	//데이터 로드 성공하면
	if (tempMesh.Succeeded())
	{
		//데이터할당
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//메시 위치 및 회전 설정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	//칼메시 설정
	SwordMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwoedMeshComp"));

	//스켈레톤 팔에 붙여주기
	SwordMeshComp->SetupAttachment(GetMesh(), "Bip01-R-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSworodMesh(TEXT("/Script/Engine.StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SWORD.SWORD'"));
	if (TempSworodMesh.Succeeded())
	{
		//4-4.스켈레탈메시 데이터할당
		SwordMeshComp->SetStaticMesh(TempSworodMesh.Object);

	}



	fsm = CreateDefaultSubobject<UEnemy_1_FSM>(TEXT("FSM"));
	//애니메이션 블루프린트 할당하기
	ConstructorHelpers::FClassFinder<UAnimInstance>tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/LJW/Enemys/SimpleSkeleton/anim/ABP_Enemy_1.ABP_Enemy_1_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}
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

