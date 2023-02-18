// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbComponent.h"
#include "VR_Player.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <MotionControllerComponent.h>
#include "HoldActor.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values for this component's properties
UClimbComponent::UClimbComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AVR_Player>(GetOwner());
}


// Called every frame
void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CanClimb)
	{
		return;
	}

	FVector delta;

	if (bClimbing_right || bClimbing_left)
	{
		// 최초 위치에서 손의 위치를 뺀 벡터를 구한다.
		delta = climb_start - currentController->GetComponentLocation();
	}

	// 앞에서 구한 벡터를 플레이어 액터의 위치에 더한다.
	player->AddActorWorldOffset(delta);
}

void UClimbComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(grab_r, ETriggerEvent::Started, this, &UClimbComponent::GrabRight);
	enhancedInputComponent->BindAction(grab_l, ETriggerEvent::Started, this, &UClimbComponent::GrabLeft);
	enhancedInputComponent->BindAction(grab_r, ETriggerEvent::Completed, this, &UClimbComponent::ReleaseRight);
	enhancedInputComponent->BindAction(grab_l, ETriggerEvent::Completed, this, &UClimbComponent::ReleaseLeft);
}

void UClimbComponent::StartClimb(bool isLeft)
{
	CanClimb = true;

	player->GetCharacterMovement()->GravityScale = 0;

	if (isLeft)
	{
		climb_start = player->motionControllerLeft->GetComponentLocation();
	}
	else
	{
		climb_start = player->motionControllerRight->GetComponentLocation();
	}

	// 	bClimbing_left = isLeft;
	// 	bClimbing_right = !isLeft;
}

void UClimbComponent::EndClimb()
{
	// 양손이 모두 놓은 상태일 때에만 실행한다.
	if (!bClimbing_left && !bClimbing_right)
	{
		CanClimb = false;
		player->GetCharacterMovement()->GravityScale = 1.0f;
		//bClimbing_right = false;
	}
}

void UClimbComponent::GrabLeft()
{
	currentController = player->motionControllerLeft;

	// 홀더를 잡았다면
	if (CheckHolder(currentController))
	{

		bClimbing_left = true;
		StartClimb(true);
	}


}

void UClimbComponent::GrabRight()
{
	currentController = player->motionControllerRight;

	if (CheckHolder(currentController) && !player->sword->IsVisible())
	{

		bClimbing_right = true;
		StartClimb(false);
	}

}

void UClimbComponent::ReleaseLeft()
{
	bClimbing_left = false;
	EndClimb();
}

void UClimbComponent::ReleaseRight()
{
	bClimbing_right = false;
	EndClimb();
}

bool UClimbComponent::CheckHolder(class UMotionControllerComponent* controller)
{
	FHitResult hitInfo;
	FVector startLoc = controller->GetComponentLocation();
	FCollisionShape cols = FCollisionShape::MakeSphere(50);

	FCollisionQueryParams params;
	params.AddIgnoredActor(player);

	FCollisionObjectQueryParams objectParams;
	objectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	bool result = false;


	if (GetWorld()->SweepSingleByObjectType(hitInfo, startLoc, startLoc, FQuat::Identity, objectParams, cols, params))
	{
		if (hitInfo.GetActor()->IsA(AHoldActor::StaticClass()))
		{
			result = true;
		}
	}
	return result;
}