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
#include <Components/CapsuleComponent.h>
#include <Components/TextRenderComponent.h>
#include "PickUpActor.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy_1.h"
#include "Enemy_1_FSM.h"
#include "Enemy_2.h"
#include "Enemy_2_FSM.h"
#include "Enemy_2_Bullet.h"
#include "PickUpFood.h"
#include <Components/SphereComponent.h>
#include "Enemy_3.h"
#include "Enemy_3_FSM.h"
#include "Enemy_4.h"
#include "Enemy_4_FSM.h"
#include "StatsUIActor.h"

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

	swordCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sword CapsuleComponent"));
	swordCapsuleComp->SetupAttachment(motionControllerRight);
	swordCapsuleComp->SetHiddenInGame(true);
	swordCapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	swordCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	swordCapsuleComp->SetGenerateOverlapEvents(true);	

	sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sword"));
	sword->SetupAttachment(swordCapsuleComp);
	sword->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	sphereCompHMD = CreateDefaultSubobject<USphereComponent>(TEXT("HMD SphereComponent"));
	sphereCompHMD->SetupAttachment(cameraComponent);	

	HMD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMD"));
	HMD->SetupAttachment(sphereCompHMD);
	HMD->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	textCompHp = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtHp"));
	textCompHp->SetupAttachment(leftHand);
	textCompHpNum = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtHpNum"));
	textCompHpNum->SetupAttachment(leftHand);

	textCompCoin = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtCoin"));
	textCompCoin->SetupAttachment(leftHand);
	textCompCoinNum = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtCoinNum"));
	textCompCoinNum->SetupAttachment(leftHand);

	textCompKey = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtkey"));
	textCompKey->SetupAttachment(leftHand);
	textCompKeyNum = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtkeyNum"));
	textCompKeyNum->SetupAttachment(leftHand);


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

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempHMDMesh(TEXT("StaticMesh'/Engine/VREditor/Devices/Generic/GenericHMD.GenericHMD'"));
	if (tempHMDMesh.Succeeded())
	{
		HMD->SetStaticMesh(tempHMDMesh.Object);
	}

	ConstructorHelpers::FClassFinder<AStatsUIActor> tempStatsUIActor(TEXT("/Script/Engine.Blueprint'/Game/KDH/Blueprints/BP_StatsUIActor.BP_StatsUIActor_C'"));
	if (tempStatsUIActor.Succeeded())
	{
		statsUIActorFactory = tempStatsUIActor.Class;
	}
	
	bUseControllerRotationPitch = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// 액터 컴포넌트들 추가
	moveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));
	equipComp = CreateDefaultSubobject<UEquipComponent>(TEXT("Equip Component"));
	graspComp = CreateDefaultSubobject<UGraspComponent>(TEXT("Grasp Component"));

	currHp = maxHp;

}

// Called when the game starts or when spawned
void AVR_Player::BeginPlay()
{
	Super::BeginPlay();

	// 헤드 장비의 기준 위치를 설정한다.
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackingOrigin.GetValue());

	// 1. 플레이어 컨트롤러를 가져온다.
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	// 2. 플레이어 컨트롤러에서 EnhancedInputLocalPlayerSubsystem 을 가져온다.
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

	// 3. 가져온 Subsystem 에 IMC 를 등록한다. (우선순위 0번)
	subsys->AddMappingContext(myMapping, 0);	

	sphereCompHMD->OnComponentBeginOverlap.AddDynamic(this, &AVR_Player::EatFood);
	swordCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AVR_Player::SwordAttack);	
	

	FString hpStr = FString::FromInt((int32)currHp);
	textCompHpNum->SetText(FText::FromString(hpStr));

	FString coinStr = FString::FromInt((int32)currCoin);
	textCompCoinNum->SetText(FText::FromString(coinStr));

	FString keyStr = FString::FromInt((int32)currKey);
	textCompKeyNum->SetText(FText::FromString(keyStr));
}

// Called every frame
void AVR_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (sword->IsVisible())
	{
		currTime += DeltaTime;
		if (currTime > createTime)
		{
			prevLocation = motionControllerRight->GetComponentLocation();
			currTime = 0;
		}
		throwDirection = motionControllerRight->GetComponentLocation() - prevLocation;

		if (throwDirection.Size() > 2)
		{
			swordCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			swordCapsuleComp->SetHiddenInGame(false);
		}
		else
		{
			swordCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			swordCapsuleComp->SetHiddenInGame(true);
		}
	}

}

// Called to bind functionality to input
void AVR_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Started, this, &AVR_Player::OpenStatsUI);
		moveComp->SetupPlayerInputComponent(enhancedInputComponent);
		equipComp->SetupPlayerInputComponent(enhancedInputComponent);
		graspComp->SetupPlayerInputComponent(enhancedInputComponent);
	}
}


void AVR_Player::SwordAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AEnemy_1* enemy_1 =  Cast<AEnemy_1>(OtherActor);
	if (enemy_1 != nullptr)
	{
		enemy_1->fsm->OnDamageProcess();
		//SetActorLocation(SweepResult.Location);
		enemy_1->GetCapsuleComponent()->AddImpulse(FVector(-50)*enemy_1->GetActorForwardVector());

	}

	AEnemy_2* enemy_2 = Cast<AEnemy_2>(OtherActor);
	if (enemy_2 != nullptr)
	{
		enemy_2->fsm->OnDamageProcess();
	}

	AEnemy_3* enemy_3 = Cast<AEnemy_3>(OtherActor);
	if (enemy_3 != nullptr)
	{
		enemy_3->fsm->OnDamageProcess();
	}

	AEnemy_4* enemy_4 = Cast<AEnemy_4>(OtherActor);
	if (enemy_4 != nullptr)
	{
		enemy_4->fsm->OnDamageProcess();
	}

	AEnemy_2_Bullet* bullet_2 = Cast<AEnemy_2_Bullet>(OtherActor);
	if (bullet_2 != nullptr)
	{
		//bullet_2->returnBack();
		GetWorld()->DestroyActor(OtherActor);
	
	}
}

void AVR_Player::ReceiveDamage()
{
	currHp--;
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0, 0.3f, 0.3, FLinearColor::Red);	

	FString hpStr = FString::FromInt((int32)currHp);
	textCompHpNum->SetText(FText::FromString(hpStr));

	if (currHp == 0)
	{		
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestMap"));
	}
}

void AVR_Player::EatFood(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	APickUpFood* food = Cast<APickUpFood>(OtherActor);
	if (food != nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("foodeat!!"));
		GetWorld()->DestroyActor(OtherActor);
		currHp++;
		FString hpStr = FString::FromInt((int32)currHp);
		textCompHpNum->SetText(FText::FromString(hpStr));
	}
}

void AVR_Player::OpenStatsUI()
{
	if (!bShowStatsUI)
	{
		statsUIActor = GetWorld()->SpawnActor<AStatsUIActor>(statsUIActorFactory, GetActorTransform());
		bShowStatsUI = true;
	}
	else
	{
		GetWorld()->DestroyActor(statsUIActor);
		bShowStatsUI = false;
	}
}




