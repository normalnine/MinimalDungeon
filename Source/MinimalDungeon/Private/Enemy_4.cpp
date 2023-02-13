// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_4.h"
#include "Enemy_4_FSM.h"
AEnemy_4::AEnemy_4()

{


ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LJW/Enemys/slime_TurtleShell/RPG_Monster_Duo_PBR_Polyart/Meshes/TurtleShellMesh.TurtleShellMesh'"));

if (tempMesh.Succeeded())
{

	GetMesh()->SetSkeletalMesh(tempMesh.Object);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
}
fsm = CreateDefaultSubobject<UEnemy_4_FSM>(TEXT("FSM"));
//애니메이션 블루프린트 할당하기
ConstructorHelpers::FClassFinder<UAnimInstance>tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/LJW/Enemys/slime_TurtleShell/RPG_Monster_Duo_PBR_Polyart/Animations/TurtleShell/ABP_Enemy_4.ABP_Enemy_4_C'"));
if (tempClass.Succeeded())
{
	GetMesh()->SetAnimInstanceClass(tempClass.Class);
}
}