// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageUIActor.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include <Components/SceneComponent.h>

// Sets default values
ADamageUIActor::ADamageUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(rootComp);
	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADamageUIActor::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ADamageUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

