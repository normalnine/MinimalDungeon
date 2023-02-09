// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_2.h"
#include "Enemy_2_FSM.h"
#include <Components/ArrowComponent.h>

// Sets default values
AEnemy_2::AEnemy_2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LJW/Enemys/plant/mesh/EnemyPlant2Environment.EnemyPlant2Environment'"));
	//데이터 로드 성공하면
	if (tempMesh.Succeeded())
	{
		//데이터할당
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}
	//Arrow1P = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));


	fsm = CreateDefaultSubobject<UEnemy_2_FSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void AEnemy_2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy_2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

