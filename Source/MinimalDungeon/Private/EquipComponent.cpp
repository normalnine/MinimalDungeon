// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "VR_Player.h"
#include "Knife.h"

// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

// 	ConstructorHelpers::FClassFinder<AKnife> tempKnife(TEXT("/Script/Engine.Blueprint'/Game/KDH/Blueprints/BP_Knife.BP_Knife_C'"));
// 	if (tempKnife.Succeeded())
// 	{
// 		knife = Cast<AKnife>(tempKnife.Class);
// 	}
}


// Called when the game starts
void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AVR_Player>(GetOwner());
}


// Called every frame
void UEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(buttonY, ETriggerEvent::Started, this, &UEquipComponent::EquipSword);
	//PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Triggered, this, &UEquipComponent::EquipKnife);
	//PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Completed, this, &UEquipComponent::EquipKnife);
}

void UEquipComponent::EquipSword()
{		
	if (isEquippingSword)
	{
		isEquippingSword = false;
		player->sword->SetVisibility(isEquippingSword);
	}
	else
	{
		isEquippingSword = true;
		player->sword->SetVisibility(isEquippingSword);
	}
}

// void UEquipComponent::EquipKnife()
// {
// 	if (isEquippingKnife)
// 	{
// 		isEquippingKnife = false;		
// 	}
// 	else
// 	{
// 		isEquippingKnife = true;
// 		GetWorld()->SpawnActor(knife, player->leftHand->GetComponentTransform());
// 		knife->AttachToComponent(player->leftHand);
// 	}
// 	
// }
