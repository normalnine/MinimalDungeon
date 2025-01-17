// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsUIActor.h"
#include <Components/SceneComponent.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include "StatsUI.h"

// Sets default values
AStatsUIActor::AStatsUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(rootComp);

	widgetComp_Controller = CreateDefaultSubobject<UWidgetComponent>(TEXT("Controller Widget Component"));
	widgetComp_Controller->SetupAttachment(rootComp);

}

// Called when the game starts or when spawned
void AStatsUIActor::BeginPlay()
{
	Super::BeginPlay();

	statsUI = Cast<UStatsUI>(widgetComp->GetWidget());

	statsUI->UpdateStats();
	
}

// Called every frame
void AStatsUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


