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

// Sets default values
AVR_Player::AVR_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetupAttachment(RootComponent);

	motionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("motionControllerRight"));
	motionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("motionControllerLeft"));
	motionControllerRight->SetupAttachment(RootComponent);
	motionControllerLeft->SetupAttachment(RootComponent);
	motionControllerLeft->MotionSource = "Left";
	motionControllerRight->MotionSource = "Right";

	rightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightHand"));
	rightHand->SetupAttachment(motionControllerRight);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	leftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftHand"));
	leftHand->SetupAttachment(motionControllerLeft);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	HMD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMD"));
	HMD->SetupAttachment(cameraComponent);
	HMD->SetCollisionEnabled(ECollisionEnabled::NoCollision);

// 	rightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("RightLogText"));
// 	rightLog->SetupAttachment(motionControllerRight);
// 	rightLog->SetRelativeRotation(FRotator(135, 0, 180));
// 	rightLog->SetRelativeLocation(FVector(0, 0, 10));
// 	rightLog->SetTextRenderColor(FColor::Yellow);
// 	rightLog->SetHorizontalAlignment(EHTA_Center);
// 	rightLog->SetHorizontalAlignment(EHTA_Center);

	leftLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LeftLogText"));
	leftLog->SetupAttachment(motionControllerLeft);
	leftLog->SetRelativeRotation(FRotator(135, 0, 180));
	leftLog->SetRelativeLocation(FVector(0, 0, 10));
	leftLog->SetTextRenderColor(FColor::Yellow);
	leftLog->SetHorizontalAlignment(EHTA_Center);



	ConstructorHelpers::FObjectFinder<UStaticMesh> tempRightMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempRightMesh.Succeeded())
	{
		rightHand->SetStaticMesh(tempRightMesh.Object);
	}
	rightHand->SetRelativeScale3D(FVector(0.1));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempLeftMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempLeftMesh.Succeeded())
	{
		leftHand->SetStaticMesh(tempLeftMesh.Object);
	}
	leftHand->SetRelativeScale3D(FVector(0.1));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempHMDMesh(TEXT("StaticMesh'/Engine/VREditor/Devices/Generic/GenericHMD.GenericHMD'"));
	if (tempHMDMesh.Succeeded())
	{
		HMD->SetStaticMesh(tempHMDMesh.Object);
	}

	bUseControllerRotationPitch = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
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
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &AVR_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &AVR_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[1], ETriggerEvent::Triggered, this, &AVR_Player::RotateAxis);
	}
}

void AVR_Player::OnTriggerLeft(const FInputActionValue& value)
{
	float val = value.Get<float>();

	// �޼� �α׿� ���� ����Ѵ�.
	FString msg = FString::Printf(TEXT("%f"), val);
	leftLog->SetText(FText::FromString(msg));
}

void AVR_Player::RotateAxis(const FInputActionValue& value)
{
	
	float val = value.Get<float>();
	// axis ���� �̿��ؼ� ĳ����(��Ʈ�ѷ�)�� ȸ���Ѵ�.
	//AddControllerPitchInput(axis.Y * -1.0f);
	AddControllerYawInput(val);


	//cam->SetRelativeRotation(FRotator(-axis.Y, 0, 0));
	//SetActorRotation(GetActorRotation()+FRotator(0,axis.X,0));
}