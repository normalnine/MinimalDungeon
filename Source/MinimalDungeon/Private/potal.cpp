// Fill out your copyright notice in the Description page of Project Settings.


#include "potal.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <Components/SphereComponent.h>
#include "VR_Player.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
Apotal::Apotal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	
	potalEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("potalEffect"));
	potalEffect->SetupAttachment(collisionComp);
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &Apotal::OnOverlapBegin);


}

// Called when the game starts or when spawned
void Apotal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Apotal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Apotal::OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVR_Player* player = Cast<AVR_Player>(otherActor);
		if (player !=nullptr)
		{

			FString Demo_Map_1;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0, 1.0f, 2.0f, FLinearColor::Black);
			//UGameplayStatics::OpenLevel(GetWorld(), FName(Demo_Map_1));
		}
}

