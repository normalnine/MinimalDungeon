// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"
#include <Components/SphereComponent.h>
#include <Engine/StaticMesh.h>
#include "VR_Player.h"
#include "GraspComponent.h"
#include "Enemy_1.h"
#include "Enemy_2.h"
#include "Enemy_3.h"
#include "Enemy_4.h"
#include "Enemy_1_FSM.h"
#include "Enemy_2_FSM.h"
#include "Enemy_3_FSM.h"
#include "Enemy_4_FSM.h"
#include "MD_GameInstance.h"
#include "Enemy_5.h"
#include "Enemy_5_FSM.h"

AKnife::AKnife()
{
}

void AKnife::BeginPlay()
{
	Super::BeginPlay();
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AKnife::KnifeAttack);
	player = Cast<AVR_Player>(GetWorld()->GetFirstPlayerController());
	gameInst = Cast<UMD_GameInstance>(GetWorld()->GetGameInstance());
}

void AKnife::KnifeAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	knifeAttackDmg += gameInst->knifeDmg;
	GetWorld()->SpawnActor<AActor>(hitEffect, OverlappedComponent->GetComponentTransform());

	if (FMath::RandRange(1, 100) <= gameInst->knifeCrit)
	{
		knifeAttackDmg *= 2;
	}
	else
	{
		knifeAttackDmg = FMath::RandRange(knifeAttackDmg / 2, knifeAttackDmg);
	}

	AEnemy_1* enemy_1 = Cast<AEnemy_1>(OtherActor);
	if (enemy_1 != nullptr)
	{	
		// 박혀 있는 동안 충돌 안되게 하기 위해 체크
		if (bKnifeStudded == false)
		{	
			enemy_1->fsm->OnDamageProcess(knifeAttackDmg);
			sphereComp->SetSimulatePhysics(false);
			SetActorLocation(enemy_1->GetActorLocation());
			bKnifeStudded = true;
		}		
	}
	
	AEnemy_2* enemy_2 = Cast<AEnemy_2>(OtherActor);
	if (enemy_2 != nullptr)
	{
		if (bKnifeStudded == false)
		{
			enemy_2->fsm->OnDamageProcess(knifeAttackDmg);
			sphereComp->SetSimulatePhysics(false);
			SetActorLocation(enemy_2->GetActorLocation());
			bKnifeStudded = true;
		}
	} 	

	AEnemy_3* enemy_3 = Cast<AEnemy_3>(OtherActor);
	if (enemy_3 != nullptr)
	{
		if (bKnifeStudded == false)
		{
			enemy_3->fsm->OnDamageProcess(knifeAttackDmg);
			sphereComp->SetSimulatePhysics(false);
			SetActorLocation(enemy_3->GetActorLocation());
			bKnifeStudded = true;
		}
	}

	AEnemy_4* enemy_4 = Cast<AEnemy_4>(OtherActor);
	if (enemy_4 != nullptr)
	{
		if (bKnifeStudded == false)
		{
			enemy_4->fsm->OnDamageProcess(knifeAttackDmg);
			sphereComp->SetSimulatePhysics(false);
			SetActorLocation(enemy_4->GetActorLocation());
			bKnifeStudded = true;
		}
	}

	AEnemy_5* enemy_5 = Cast<AEnemy_5>(OtherActor);
	if (enemy_5 != nullptr)
	{
		if (bKnifeStudded == false)
		{
			enemy_5->fsm->OnDamageProcess(knifeAttackDmg);
			sphereComp->SetSimulatePhysics(false);
			SetActorLocation(enemy_5->GetActorLocation());
			bKnifeStudded = true;
		}
	}
}