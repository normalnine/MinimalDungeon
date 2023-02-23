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
/*#include "EquipComponent.h"*/
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
#include <UMG/Public/Components/WidgetInteractionComponent.h>
#include "WidgetPointerComponent.h"
#include "MD_GameInstance.h"
#include "ClimbComponent.h"
#include "BuyComponent.h"
#include "Enemy_5.h"
#include "Enemy_5_FSM.h"
#include <Sound/SoundCue.h>
#include <Sound/SoundWave.h>
#include <Components/AudioComponent.h>
#include "Engine/GameViewportClient.h"
#include "GameFramework/PlayerController.h"
#include <Haptics/HapticFeedbackEffect_Base.h>
#include "Engine/World.h"
#include "DamageUIActor.h"
#include "DamageUI.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include <UMG/Public/Components/TextBlock.h>

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

	sphereCompLeftHand = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent LeftHand"));
	sphereCompLeftHand->SetupAttachment(motionControllerLeft);
	sphereCompLeftHand->SetCollisionProfileName(FName("VRPlayerHand"));

	sphereCompRightHand = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent RightHand"));
	sphereCompRightHand->SetupAttachment(motionControllerRight);
	sphereCompRightHand->SetCollisionProfileName(FName("VRPlayerHand"));


	leftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftHand"));
	leftHand->SetupAttachment(motionControllerLeft);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	leftHand->SetRelativeScale3D(FVector(0.1));

	rightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightHand"));
	rightHand->SetupAttachment(motionControllerRight);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeScale3D(FVector(0.1));

	widgetPointer_right = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Pointer"));
	widgetPointer_right->SetupAttachment(motionControllerRight);

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

// 	textCompHp = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtHp"));
// 	textCompHp->SetupAttachment(leftHand);
	meshCompHp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp HP"));
	meshCompHp->SetupAttachment(leftHand);
	meshCompHp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	textCompHpNum = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtHpNum"));
	textCompHpNum->SetupAttachment(leftHand);

// 	textCompCoin = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtCoin"));
// 	textCompCoin->SetupAttachment(leftHand);
	meshCompCoin = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp Coin"));
	meshCompCoin->SetupAttachment(leftHand);
	meshCompCoin->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	textCompCoinNum = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtCoinNum"));
	textCompCoinNum->SetupAttachment(leftHand);

// 	textCompKey = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtkey"));
// 	textCompKey->SetupAttachment(leftHand);
	meshCompKey = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp Key"));
	meshCompKey->SetupAttachment(leftHand);
	meshCompKey->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	textCompKeyNum = CreateDefaultSubobject<UTextRenderComponent>(TEXT("txtkeyNum"));
	textCompKeyNum->SetupAttachment(leftHand);

	heartbeatSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	heartbeatSound->SetupAttachment(cameraComponent);

	swordAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Sword Audio Component"));
	swordAudio->SetupAttachment(swordCapsuleComp);

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
// 	equipComp = CreateDefaultSubobject<UEquipComponent>(TEXT("Equip Component"));
	graspComp = CreateDefaultSubobject<UGraspComponent>(TEXT("Grasp Component"));
	widgetPointerComp = CreateDefaultSubobject<UWidgetPointerComponent>(TEXT("Widget Pointer Component"));
	climbComp = CreateDefaultSubobject<UClimbComponent>(TEXT("Climb Component"));
	buyComp = CreateDefaultSubobject<UBuyComponent>(TEXT("Buy Component"));
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

	gameInst = Cast<UMD_GameInstance>(GetWorld()->GetGameInstance());

	sphereCompHMD->OnComponentBeginOverlap.AddDynamic(this, &AVR_Player::EatFood);
	swordCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AVR_Player::SwordAttack);	
	

	textCompHpNum->SetText(FText::AsNumber(gameInst->hp));
	textCompCoinNum->SetText(FText::AsNumber(gameInst->coin));
	textCompKeyNum->SetText(FText::AsNumber(gameInst->key));

	//heartbeatSound->Stop();

	playerController = GetWorld()->GetFirstPlayerController();

	
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

		if (throwDirection.Length() > 30)
		{
			swordCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			swordCapsuleComp->SetHiddenInGame(false);
			if (!swordAudio->IsPlaying())
			{
				swordAudio->Play(0);
			}
			

// 			soundTime += DeltaTime;
// 			if (soundTime > playTime)
// 			{
// 				UGameplayStatics::PlaySound2D(GetWorld(), swordSound);
// 				soundTime = 0;
// 			}
		}
		else
		{
			swordCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			swordCapsuleComp->SetHiddenInGame(true);
		}
	}
	else
	{
		swordCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		swordCapsuleComp->SetHiddenInGame(true);
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
		enhancedInputComponent->BindAction(buttonB, ETriggerEvent::Started, this, &AVR_Player::EquipSword);
		enhancedInputComponent->BindAction(rightThumbStick, ETriggerEvent::Started, this, &AVR_Player::Recenter);

		moveComp->SetupPlayerInputComponent(enhancedInputComponent);
// 		equipComp->SetupPlayerInputComponent(enhancedInputComponent);
		graspComp->SetupPlayerInputComponent(enhancedInputComponent);
		widgetPointerComp->SetupPlayerInputComponent(enhancedInputComponent);
		climbComp->SetupPlayerInputComponent(enhancedInputComponent);
		buyComp->SetupPlayerInputComponent(enhancedInputComponent);


	}
}


void AVR_Player::SwordAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	swordAttackDmg += gameInst->swordDmg;
	GetWorld()->SpawnActor<AActor>(hitEffect, OverlappedComponent->GetComponentTransform());
	playerController->SetHapticsByValue(1.0f, 1.0f, EControllerHand::Right);
	if (FMath::RandRange(1, 100) <= gameInst->swordCrit)
	{
		swordAttackDmg *= 2;
	}
	else
	{
		swordAttackDmg = FMath::RandRange(swordAttackDmg/2, swordAttackDmg);
	}
	
	AEnemy_1* enemy_1 =  Cast<AEnemy_1>(OtherActor);
	if (enemy_1 != nullptr)
	{
		SpawnDamageUI(OtherActor);
		enemy_1->fsm->OnDamageProcess(swordAttackDmg);
		

		
	}

	AEnemy_2* enemy_2 = Cast<AEnemy_2>(OtherActor);
	if (enemy_2 != nullptr)
	{	
		SpawnDamageUI(OtherActor);
		enemy_2->fsm->OnDamageProcess(swordAttackDmg);
		
	}

	AEnemy_3* enemy_3 = Cast<AEnemy_3>(OtherActor);
	if (enemy_3 != nullptr)
	{	
		SpawnDamageUI(OtherActor);
		enemy_3->fsm->OnDamageProcess(swordAttackDmg);
	}

	AEnemy_4* enemy_4 = Cast<AEnemy_4>(OtherActor);
	if (enemy_4 != nullptr)
	{
		SpawnDamageUI(OtherActor);
		enemy_4->fsm->OnDamageProcess(swordAttackDmg);
	}

	AEnemy_2_Bullet* bullet_2 = Cast<AEnemy_2_Bullet>(OtherActor);
	if (bullet_2 != nullptr)
	{
		//bullet_2->returnBack();
		GetWorld()->DestroyActor(OtherActor);
	
	}

	AEnemy_5* enemy_5 = Cast<AEnemy_5>(OtherActor);
	if (enemy_5 != nullptr)
	{
		SpawnDamageUI(OtherActor);
		enemy_5->fsm->OnDamageProcess(swordAttackDmg);
	}
	swordAttackDmg = 0;
}

void AVR_Player::ReceiveDamage()
{
	if (FMath::RandRange(1, 100) <= gameInst->evasion)
	{
		return;
	}
	gameInst->hp--;
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0, 0.3f, 0.3, FLinearColor::Red);	
	textCompHpNum->SetText(FText::AsNumber(gameInst->hp));

	if (gameInst->hp < 3 && !heartbeatSound->IsPlaying())
	{		
		heartbeatSound->Play(0);
	}

	if (gameInst->hp < 1)
	{	
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0, 1.0f, 1.0f, FLinearColor::Red);
		FTimerHandle deathTimer;
		GetWorldTimerManager().SetTimer(deathTimer, this, &AVR_Player::Die, 1.5f);		
	}
}

void AVR_Player::EatFood(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	APickUpFood* food = Cast<APickUpFood>(OtherActor);
	if (food != nullptr)
	{	
		UGameplayStatics::PlaySound2D(GetWorld(),eatingSound);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0, 0.3f, 0.5, FLinearColor::Green);
		GetWorld()->DestroyActor(food);
		gameInst->hp++;
		textCompHpNum->SetText(FText::AsNumber(gameInst->hp));
		if (gameInst->hp > 2)
		{
			heartbeatSound->Stop();
		}
	}
}

void AVR_Player::OpenStatsUI()
{
	if (!bShowStatsUI)
	{
		statsUIActor = GetWorld()->SpawnActor<AStatsUIActor>(statsUIActorFactory, GetActorTransform());
		UGameplayStatics::PlaySound2D(GetWorld(), openUISound);
		widgetPointer_right->bShowDebug = true;
		bShowStatsUI = true;
	}
	else
	{
		GetWorld()->DestroyActor(statsUIActor);
		widgetPointer_right->bShowDebug = false;
		bShowStatsUI = false;
	}
}

void AVR_Player::EquipSword()
{
	if (isEquippingSword)
	{
		isEquippingSword = false;
		sword->SetVisibility(isEquippingSword);

	}
	else if (!isEquippingSword && !graspComp->bIsGrab && !climbComp->bClimbing_right)
	{
		isEquippingSword = true;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), equipSound, rightHand->GetComponentLocation());
		sword->SetVisibility(isEquippingSword);
	}
}

void AVR_Player::Recenter()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVR_Player::Die()
{
	gameInst->StatsInit();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Script/Engine.World'/Game/StylizedDungeonVol2/Maps/Demo_Map_1.Demo_Map_1'"));
}



void AVR_Player::SpawnDamageUI(AActor* OtherActor)
{
	FVector dir = GetActorLocation() - OtherActor->GetActorLocation();
	dir.Normalize();
	damageUIActor = GetWorld()->SpawnActor<ADamageUIActor>(damageUIActorFactory, OtherActor->GetActorLocation(), dir.Rotation());
	damageUI = Cast<UDamageUI>(damageUIActor->widgetComp->GetWidget());
	if (FMath::RandRange(1, 100) <= gameInst->swordCrit)
	{
		swordAttackDmg *= 2;
		damageUI->damage->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.0f));
	}
	else
	{
		swordAttackDmg = FMath::RandRange(swordAttackDmg / 2, swordAttackDmg);
		damageUI->damage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f));

	}
	damageUI->damage->SetText(FText::AsNumber(swordAttackDmg));
}

