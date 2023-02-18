// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_5.h"
#include "Enemy_5_FSM.h"

AEnemy_5::AEnemy_5()
{


	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LJW/Enemys/ghost/models/Ghost_Lv1.Ghost_Lv1'"));

	if (tempMesh.Succeeded())
	{

		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	fsm = CreateDefaultSubobject<UEnemy_5_FSM>(TEXT("FSM"));
}