// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"
#include <Components/SphereComponent.h>
#include <Engine/StaticMesh.h>
#include "VR_Player.h"
#include "GraspComponent.h"
#include "Enemy_1.h"
#include "Enemy_2.h"
#include "Enemy_1_FSM.h"
#include "Enemy_2_FSM.h"

AKnife::AKnife()
{
	sphereComp->SetSphereRadius(10);
	meshComp->SetRelativeScale3D(FVector(0.03));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempKnifeMesh(TEXT("/Script/Engine.StaticMesh'/Engine/VREditor/BasicMeshes/SM_Cube_03.SM_Cube_03'"));
	if (tempKnifeMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempKnifeMesh.Object);
	}
}

void AKnife::BeginPlay()
{
	Super::BeginPlay();
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AKnife::KnifeAttack);
	player = Cast<AVR_Player>(GetWorld()->GetFirstPlayerController());
}

void AKnife::KnifeAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy_1* enemy_1 = Cast<AEnemy_1>(OtherActor);
	if (enemy_1 != nullptr)
	{
		if (bKnifeStudded == false)
		{
			enemy_1->fsm->OnDamageProcess();
			sphereComp->SetSimulatePhysics(false);
			SetActorLocation(enemy_1->GetActorLocation());
			//SetActorLocation(SweepResult.Location);
			bKnifeStudded = true;
		}		
	}
	
	AEnemy_2* enemy_2 = Cast<AEnemy_2>(OtherActor);
	if (enemy_2 != nullptr)
	{
		if (bKnifeStudded == false)
		{
			enemy_2->fsm->OnDamageProcess();
			sphereComp->SetSimulatePhysics(false);
			SetActorLocation(enemy_2->GetActorLocation());
			//SetActorLocation(SweepResult.Location);
			bKnifeStudded = true;
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("hit!!!!!!!!!!!")));
 	
}