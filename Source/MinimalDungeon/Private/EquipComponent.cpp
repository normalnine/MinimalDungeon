// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "VR_Player.h"
#include "ClimbComponent.h"
#include "GraspComponent.h"

// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;	
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
	
}

void UEquipComponent::EquipSword()
{		
	if (isEquippingSword)
	{
		isEquippingSword = false;
		player->sword->SetVisibility(isEquippingSword);

	}
	else if(!isEquippingSword && !player->graspComp->bIsGrab && !player->climbComp->bClimbing_right)
	{
		isEquippingSword = true;
		player->sword->SetVisibility(isEquippingSword);
	}
}


