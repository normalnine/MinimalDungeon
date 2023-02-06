// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include <Components/BoxComponent.h>

// Sets default values
AKnife::AKnife()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionProfileName(TEXT("Knife"));

	staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Knife Mesh"));
	staticMeshComp->SetRelativeScale3D(FVector(0.5, 0.05, 0.05));
	staticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	staticMeshComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempKnifeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempKnifeMesh.Succeeded())
	{
		staticMeshComp->SetStaticMesh(tempKnifeMesh.Object);
	}
	

}

// Called when the game starts or when spawned
void AKnife::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

