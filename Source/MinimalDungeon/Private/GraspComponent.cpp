// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "VR_Player.h"


// Sets default values for this component's properties
UGraspComponent::UGraspComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGraspComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AVR_Player>(GetOwner());
	
}


// Called every frame
void UGraspComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
void UGraspComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(grip_left, ETriggerEvent::Triggered, this, &UGraspComponent::GripLeftAction);
	PlayerInputComponent->BindAction(grip_left, ETriggerEvent::Completed, this, &UGraspComponent::GripLeftAction);
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Triggered, this, &UGraspComponent::GripRightAction);
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Completed, this, &UGraspComponent::GripRightAction);
}

void UGraspComponent::GripLeftAction(const struct FInputActionValue& value)
{
	GrapObject(player->leftHand);
}
void UGraspComponent::GripRightAction(const struct FInputActionValue& value)
{
	//GrapObject(player->rightHand);
}

// 물체를 잡는 함수
void UGraspComponent::GrapObject(UStaticMeshComponent* selectHand)
{
	// 방법 1 - LineTrace 방식
	FVector startLoc = selectHand->GetComponentLocation();
	FVector endLoc = startLoc + selectHand->GetRightVector() * grapDistance;
	FHitResult hitInfo;

	if (GetWorld()->LineTraceSingleByProfile(hitInfo, startLoc, endLoc, TEXT("Knife")))
	{
		hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::KeepWorldTransform);
	}
}