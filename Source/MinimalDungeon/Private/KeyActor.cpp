// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyActor.h"
#include "VR_Player.h"
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Math/UnrealMathUtility.h"
#include "MD_GameInstance.h"
#include <Components/TextRenderComponent.h>

// Sets default values
AKeyActor::AKeyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	keybody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	keybody->SetupAttachment(RootComponent);

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	collisionComp->SetupAttachment(keybody);
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AKeyActor::OnOverlapBegin);

	ConstructorHelpers::FObjectFinder<USoundBase> TSound(TEXT("/Script/Engine.SoundWave'/Game/LJW/sound/effect/confirm_style_6_001.confirm_style_6_001'"));
	if (TSound.Succeeded())
	{
		Sound = TSound.Object;
	}
}

// Called when the game starts or when spawned
void AKeyActor::BeginPlay()
{
	Super::BeginPlay();
	
	player = Cast<AVR_Player>(GetWorld()->GetFirstPlayerController());
}

// Called every frame
void AKeyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canMove)
	{
		movetotarget();
	}

	
}

void AKeyActor::OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	player = Cast<AVR_Player>(otherActor);
	if (player != nullptr)
	{
		
		canMove = true;
		FTimerHandle aaa;
		GetWorld()->GetTimerManager().SetTimer(aaa,this,&AKeyActor::moveStop,1.5f,false);
		
	}
}

void AKeyActor::movetotarget()
{
	if (canMove)
	{

		FVector myLoc = GetActorLocation();
		FVector playerLoc = player->GetActorLocation();
		SetActorLocation(FMath::VInterpTo(myLoc, playerLoc, GetWorld()->DeltaTimeSeconds, 1.0f));


	}
	
}

void AKeyActor::moveStop()
{
	UMD_GameInstance* gameInst = Cast<UMD_GameInstance>(GetWorld()->GetGameInstance());
	gameInst->key++;
	player->textCompKeyNum->SetText(FText::AsNumber(gameInst->key));
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
	Destroy();
}

