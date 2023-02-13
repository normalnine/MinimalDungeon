// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyF.h"

// Sets default values
AEnemyF::AEnemyF()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyF::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyF::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyF::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

