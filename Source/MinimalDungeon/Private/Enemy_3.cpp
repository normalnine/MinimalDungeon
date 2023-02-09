// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_3.h"

// Sets default values
AEnemy_3::AEnemy_3()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy_3::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy_3::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

