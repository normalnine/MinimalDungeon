// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpFood.h"
#include <UMG/Public/Components/WidgetComponent.h>

#include "PickUpFoodUI.h"


APickUpFood::APickUpFood()
{
	widgetComp = CreateDefaultSubobject<UWidgetComponent>("Widget Component");
	widgetComp->SetupAttachment(RootComponent);
	widgetComp->SetDrawSize(FVector2D(500));
	widgetComp->SetRelativeRotation(FRotator(0, 180, 0));
	widgetComp->SetRelativeLocation(FVector(40, 0, 80));
	widgetComp->SetRelativeScale3D(FVector(0.1));
}

void APickUpFood::BeginPlay()
{
	Super::BeginPlay();

	foodUI = Cast<UPickUpFoodUI>(widgetComp->GetWidget());
	foodUI->UpdateInfo(foodName,foodEffect);
}