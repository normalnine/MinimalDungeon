// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_Player.h"
#include <MotionControllerComponent.h>
#include <Camera/CameraComponent.h>
#include <Engine/StaticMesh.h>
#include <Components/StaticMeshComponent.h>
#include <HeadMountedDisplayFunctionLibrary.h>
#include <Components/TextRenderComponent.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "MoveComponent.h"
#include "EquipComponent.h"
#include "GraspComponent.h"

// Sets default values
AVR_Player::AVR_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetupAttachment(RootComponent);

	motionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("motionControllerLeft"));
	motionControllerLeft->SetupAttachment(RootComponent);
	motionControllerLeft->MotionSource = "Left";

	motionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("motionControllerRight"));
	motionControllerRight->SetupAttachment(RootComponent);
	motionControllerRight->MotionSource = "Right";

	leftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftHand"));
	leftHand->SetupAttachment(motionControllerLeft);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	leftHand->SetRelativeScale3D(FVector(0.1));

	rightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightHand"));
	rightHand->SetupAttachment(motionControllerRight);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeScale3D(FVector(0.1));

	sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sword"));
	sword->SetupAttachment(motionControllerRight);
	sword->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);;
	sword->SetRelativeScale3D(FVector(0.5, 0.05, 0.05));
	sword->SetRelativeLocation(FVector(30, 0, 0));

	HMD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMD"));
	HMD->SetupAttachment(cameraComponent);
	HMD->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempLeftMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempLeftMesh.Succeeded())
	{
		leftHand->SetStaticMesh(tempLeftMesh.Object);
	}	

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempRightMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempRightMesh.Succeeded())
	{
		rightHand->SetStaticMesh(tempRightMesh.Object);
	}	

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempswordMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempswordMesh.Succeeded())
	{
		sword->SetStaticMesh(tempswordMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempHMDMesh(TEXT("StaticMesh'/Engine/VREditor/Devices/Generic/GenericHMD.GenericHMD'"));
	if (tempHMDMesh.Succeeded())
	{
		HMD->SetStaticMesh(tempHMDMesh.Object);
	}

	bUseControllerRotationPitch = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// ���� ������Ʈ�� �߰�
	moveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));
	equipComp = CreateDefaultSubobject<UEquipComponent>(TEXT("Equip Component"));
	graspComp = CreateDefaultSubobject<UGraspComponent>(TEXT("Grasp Component"));

}

// Called when the game starts or when spawned
void AVR_Player::BeginPlay()
{
	Super::BeginPlay();

	// ��� ����� ���� ��ġ�� �����Ѵ�.
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackingOrigin.GetValue());

	// 1. �÷��̾� ��Ʈ�ѷ��� �����´�.
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	// 2. �÷��̾� ��Ʈ�ѷ����� EnhancedInputLocalPlayerSubsystem �� �����´�.
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

	// 3. ������ Subsystem �� IMC �� ����Ѵ�. (�켱���� 0��)
	subsys->AddMappingContext(myMapping, 0);	
}

// Called every frame
void AVR_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVR_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		moveComp->SetupPlayerInputComponent(enhancedInputComponent);
		equipComp->SetupPlayerInputComponent(enhancedInputComponent);
		graspComp->SetupPlayerInputComponent(enhancedInputComponent);
	}
}





