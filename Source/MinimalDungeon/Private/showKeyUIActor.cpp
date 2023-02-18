// Fill out your copyright notice in the Description page of Project Settings.


#include "showKeyUIActor.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include "showKeyUI.h"


// Sets default values
AshowKeyUIActor::AshowKeyUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	wcomp = CreateDefaultSubobject<UWidgetComponent>(TEXT("wcomp"));
}

// Called when the game starts or when spawned
void AshowKeyUIActor::BeginPlay()
{
	Super::BeginPlay();
	key = Cast<UshowKeyUI>(wcomp->GetWidget());
}

// Called every frame
void AshowKeyUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

