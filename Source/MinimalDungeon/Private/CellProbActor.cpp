// Fill out your copyright notice in the Description page of Project Settings.


#include "CellProbActor.h"
#include <Components/SphereComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include "VR_Player.h"
#include "CellProbUI.h"
#include "MD_GameInstance.h"
#include <Sound/SoundBase.h>

// Sets default values
ACellProbActor::ACellProbActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	sphereComp->SetCollisionProfileName(FName("CellProb"));
	sphereComp->SetSphereRadius(10);
	SetRootComponent(sphereComp);
	
	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh Component"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	meshComp->SetRelativeScale3D(FVector(20));

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(RootComponent);
	widgetComp->SetDrawSize(FVector2D(500));
	widgetComp->SetRelativeRotation(FRotator(0, 180, 0));
	widgetComp->SetRelativeLocation(FVector(20,0,40));
	widgetComp->SetRelativeScale3D(FVector(0.1));
	widgetComp->SetVisibility(false);
}

// Called when the game starts or when spawned
void ACellProbActor::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACellProbActor::OverlapHand);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &ACellProbActor::OverlapEnd);

	player = Cast<AVR_Player>(GetWorld()->GetFirstPlayerController());

	gameInst = Cast<UMD_GameInstance>(GetWorld()->GetGameInstance());
}

// Called every frame
void ACellProbActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACellProbActor::OverlapHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	UE_LOG(LogTemp,Warning, TEXT("ItemOverlap!!"));
	cellProbUI = Cast<UCellProbUI>(widgetComp->GetWidget());
	cellProbUI->UpdateInfo(price, probName, probEffect);
	widgetComp->SetVisibility(true);
}

void ACellProbActor::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	widgetComp->SetVisibility(false);
}

void ACellProbActor::PotionEffect()
{
	return;
}


