// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_3.h"
#include "Enemy_3_FSM.h"

AEnemy_3::AEnemy_3()
{


	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LJW/Enemys/slime_TurtleShell/RPG_Monster_Duo_PBR_Polyart/Meshes/SlimeMesh.SlimeMesh'"));
	
	if (tempMesh.Succeeded())
	{
	
	GetMesh()->SetSkeletalMesh(tempMesh.Object);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}


	fsm = CreateDefaultSubobject<UEnemy_3_FSM>(TEXT("FSM"));

	//애니메이션 블루프린트 할당하기
	ConstructorHelpers::FClassFinder<UAnimInstance>tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/LJW/Enemys/slime_TurtleShell/RPG_Monster_Duo_PBR_Polyart/Animations/Slime/ABP_Enemy_3.ABP_Enemy_3_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}
}