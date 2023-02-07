// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "VR_Player.h"
#include "DrawDebugHelpers.h"
#include "PickUpActor.h"
#include <Components/TextRenderComponent.h>
#include <Components/SphereComponent.h>
#include "Knife.h"


// Sets default values for this component's properties
UGraspComponent::UGraspComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AKnife> tempKnife(TEXT("/Script/Engine.Blueprint'/Game/KDH/Blueprints/BP_Knife.BP_Knife_C'"));
	if (tempKnife.Succeeded())
	{
		knifeFactory = tempKnife.Class;
	}
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

	if (bIsGrab)
	{
		DrawGrabRange();
		if (IsValid(grabedObject))
		{
			prevLocation = grabedObject->GetActorLocation();
		}

	}
}
void UGraspComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(grip_left, ETriggerEvent::Started, this, &UGraspComponent::GripLeftAction);
	PlayerInputComponent->BindAction(grip_left, ETriggerEvent::Completed, this, &UGraspComponent::GripLeftRelease);
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Started, this, &UGraspComponent::GripRightAction);
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Completed, this, &UGraspComponent::GripRightRelease);

	PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Triggered, this, &UGraspComponent::EquipKnife);
	//PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Triggered, this, &UGraspComponent::GrabKnife);
	PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Completed, this, &UGraspComponent::GripLeftRelease);
}

void UGraspComponent::GripLeftAction(const struct FInputActionValue& value)
{
	GrapObject(player->leftHand);
}
void UGraspComponent::GripLeftRelease(const struct FInputActionValue& value)
{
	bIsGrab = false;
	ReleaseObject(player->leftHand);
}

void UGraspComponent::GripRightAction(const struct FInputActionValue& value)
{
	return;
	//GrapObject(player->rightHand);
}
void UGraspComponent::GripRightRelease(const struct FInputActionValue& value)
{
	return;
}

// 물체를 잡는 함수
void UGraspComponent::GrapObject(UStaticMeshComponent* selectHand)
{
	FVector Center = selectHand->GetComponentLocation();
	FHitResult hitInfo;
	FString profileName = FString(TEXT("PickUp"));
	FCollisionQueryParams params;
	params.AddIgnoredActor(player);

	if (GetWorld()->SweepSingleByProfile(hitInfo, Center, Center, FQuat::Identity, FName(*profileName), FCollisionShape::MakeSphere(grapDistance), params) && grabedObject == nullptr)
	{
		//player->rightLog->SetText(FText::FromString(hitInfo.GetActor()->GetName()));
		grabedObject = Cast<APickUpActor>(hitInfo.GetActor());

		if (IsValid(grabedObject))
		{
			if (grabedObject == knife && isEquippingKnife == true)
			{
				knife->sphereComp->SetSimulatePhysics(true);
			}
			// 만일, 검색된 물체의 physics 가 simulate 중이라면...
			bPhysicsState = grabedObject->sphereComp->IsSimulatingPhysics();
			
			if (bPhysicsState)
			{
				// 대상의 physics 를 꺼준다.
				grabedObject->sphereComp->SetSimulatePhysics(false);
			}

			hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::KeepWorldTransform);
		}

	}

	bIsGrab = true;
}

void UGraspComponent::ReleaseObject(UStaticMeshComponent* selectHand)
{
	if (grabedObject == knife && selectHand->GetPhysicsLinearVelocityAtPoint(prevLocation).Size() < 0.1)
	{
		GetWorld()->DestroyActor(grabedObject);
	 	isEquippingKnife = false;
	}

	// 만일, 잡고 있던 물체가 있다면...
	if (grabedObject != nullptr)
	{
		// 잡고 있던 물체 때어낸다
		grabedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// 물체의 본래 피직스 on/off 여부를 되돌려준다.

		grabedObject->sphereComp->SetSimulatePhysics(bPhysicsState);

		//FVector dir = grabedObject->GetActorLocation() - prevLocation;
		//dir.GetSafeNormal();
		//FVector p = grabedObject->GetActorLocation() + dir * GetWorld()->DeltaTimeSeconds * throwPower;

		

		FVector dir = selectHand->GetPhysicsLinearVelocity();
		grabedObject->sphereComp->AddImpulse(dir * throwPower);

		FVector dTheta = selectHand->GetPhysicsAngularVelocityInDegrees();
		grabedObject->sphereComp->AddTorqueInDegrees(dTheta * throwPower);

		// grabedObject 포인터 변수를 nullptr 로 변경한다
		grabedObject = nullptr;
	}
}

void UGraspComponent::DrawGrabRange()
{
	// 선 그리기 (Sphere)
	DrawDebugSphere(GetWorld(), player->leftHand->GetComponentLocation(), grapDistance, 30, FColor::Cyan, false, -1, 0, 1);
}

void UGraspComponent::EquipKnife()
{
	if (isEquippingKnife)
	{
		knife->sphereComp->SetSimulatePhysics(false);
		FVector dir = player->leftHand->GetComponentLocation() - knife->GetActorLocation();
		dir.GetSafeNormal();
		FVector p = knife->GetActorLocation() + dir * returnSpeed* GetWorld()->DeltaTimeSeconds;
		knife->SetActorLocation(p);
	
		
	}
	else
	{
		isEquippingKnife = true;
		knife = GetWorld()->SpawnActor<AKnife>(knifeFactory, player->leftHand->GetComponentTransform());
		
	}
	GrapObject(player->leftHand);
}