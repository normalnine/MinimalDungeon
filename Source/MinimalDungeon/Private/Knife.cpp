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
#include "DamageUIActor.h"
#include "DamageUI.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include <UMG/Public/Components/TextBlock.h>
#include <Kismet/GameplayStatics.h>

AKnife::AKnife()
{
}

void AKnife::BeginPlay()
{
	Super::BeginPlay();
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AKnife::KnifeAttack);
	gameInst = Cast<UMD_GameInstance>(GetWorld()->GetGameInstance());
}

void AKnife::KnifeAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),AVR_Player::StaticClass());
	player = Cast<AVR_Player>(actor);
	knifeAttackDmg += gameInst->knifeDmg;
	GetWorld()->SpawnActor<AActor>(hitEffect, OverlappedComponent->GetComponentTransform());

	

	AEnemy_1* enemy_1 = Cast<AEnemy_1>(OtherActor);
	if (enemy_1 != nullptr)
	{	
		// 박혀 있는 동안 충돌 안되게 하기 위해 체크
		if (bKnifeStudded == false)
		{	
			SpawnDamageUI(OtherActor);
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
			SpawnDamageUI(OtherActor);
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
			SpawnDamageUI(OtherActor);
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
			SpawnDamageUI(OtherActor);
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
			SpawnDamageUI(OtherActor);
			enemy_5->fsm->OnDamageProcess(knifeAttackDmg);
			sphereComp->SetSimulatePhysics(false);
			SetActorLocation(enemy_5->GetActorLocation());			
			bKnifeStudded = true;
		}
	}
	knifeAttackDmg = 0;
}

void AKnife::SpawnDamageUI(AActor* OtherActor)
{
	FVector dir = player->GetActorLocation() - OtherActor->GetActorLocation();
	dir.Normalize();
	damageUIActor = GetWorld()->SpawnActor<ADamageUIActor>(damageUIActorFactory, OtherActor->GetActorLocation(), dir.Rotation());
	damageUI = Cast<UDamageUI>(damageUIActor->widgetComp->GetWidget());
	if (FMath::RandRange(1, 100) <= gameInst->knifeCrit)
	{
		knifeAttackDmg *= 2;
		damageUI->damage->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.0f));
	}
	else
	{
		knifeAttackDmg = FMath::RandRange(knifeAttackDmg / 2, knifeAttackDmg);
		damageUI->damage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f));

	}
	damageUI->damage->SetText(FText::AsNumber(knifeAttackDmg));	
}

