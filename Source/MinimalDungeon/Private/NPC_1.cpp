// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_1.h"

// Sets default values
ANPC_1::ANPC_1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	//������ �ε� �����ϸ�
	if (tempMesh.Succeeded())
	{
		//�������Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//�޽� ��ġ �� ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
}

// Called when the game starts or when spawned
void ANPC_1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC_1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

