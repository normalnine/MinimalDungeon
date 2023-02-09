// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"
#include <Components/SphereComponent.h>
#include <Engine/StaticMesh.h>
#include "VR_Player.h"
#include "GraspComponent.h"

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
	sphereComp->OnComponentHit.AddDynamic(this, &AKnife::KnifeAttack);
	player = Cast<AVR_Player>(GetWorld()->GetFirstPlayerController());
}

void AKnife::KnifeAttack(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
// 	if (player->graspComp->bIsGrab)
// 	{
// 		return;
// 	}
// 	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("hit!!!!!!!!!!!")));
// 	SetActorLocation(Hit.Location);
}