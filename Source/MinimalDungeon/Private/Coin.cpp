// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "VR_Player.h"
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Math/UnrealMathUtility.h"
#include "MD_GameInstance.h"
#include <Components/TextRenderComponent.h>

// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(RootComponent);

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	collisionComp->SetupAttachment(bodyMeshComp);
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<AVR_Player>(GetWorld()->GetFirstPlayerController());
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (canMove)
	{
		movetotarget();
	}
}

void ACoin::OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	player = Cast<AVR_Player>(otherActor);
	if (player != nullptr)
	{

		canMove = true;
		FTimerHandle aaa;
		GetWorld()->GetTimerManager().SetTimer(aaa, this, &ACoin::moveStop, 1.5f, false);

	}
}

void ACoin::movetotarget()
{
	if (canMove)
	{

		FVector myLoc = GetActorLocation();
		FVector playerLoc = player->GetActorLocation();
		SetActorLocation(FMath::VInterpTo(myLoc, playerLoc, GetWorld()->DeltaTimeSeconds, 1.0f));


	}

}

void ACoin::moveStop()
{
	
	
		UMD_GameInstance* gameInst = Cast<UMD_GameInstance>(GetWorld()->GetGameInstance());
		gameInst->coin++;
		player->textCompCoinNum->SetText(FText::AsNumber(gameInst->coin));
		Destroy();
	
	
}

