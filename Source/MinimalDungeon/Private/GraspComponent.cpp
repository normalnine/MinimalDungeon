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
#include <MotionControllerComponent.h>


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
			currTime += DeltaTime;
			if (currTime > createTime)
			{				
				prevLocation = player->motionControllerLeft->GetComponentLocation();
				prevForward = player->motionControllerLeft->GetForwardVector();
				currTime = 0;

 			}
			throwDirection = player->motionControllerLeft->GetComponentLocation() - prevLocation;

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
	ReleaseObject();
	
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

// ��ü�� ��� �Լ�
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
				knife->bKnifeStudded = false;
			}
			// ����, �˻��� ��ü�� physics �� simulate ���̶��...
			bPhysicsState = grabedObject->sphereComp->IsSimulatingPhysics();
			
			if (bPhysicsState)
			{
				// ����� physics �� ���ش�.
				grabedObject->sphereComp->SetSimulatePhysics(false);
			}

			hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::KeepWorldTransform);
			grabedObject->SetActorRotation(selectHand->GetComponentRotation());
		}

	}

	bIsGrab = true;
}

void UGraspComponent::ReleaseObject()
{		
	// ����, ��� �ִ� ��ü�� �ִٸ�...
	if (grabedObject != nullptr)
	{
		// ���� �
		
		if (grabedObject == knife && throwDirection.Size() <10)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("%.3f, %.3f, %.3f"), throwDirection.X, throwDirection.Y, throwDirection.Z));

			GetWorld()->DestroyActor(grabedObject);
			isEquippingKnife = false;
			grabedObject = nullptr;
			return;
		}
		throwDirection.Normalize();
		// ��� �ִ� ��ü �����
		grabedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// ��ü�� ���� ������ on/off ���θ� �ǵ����ش�.

		grabedObject->sphereComp->SetSimulatePhysics(bPhysicsState);

		//FVector dir = grabedObject->GetActorLocation() - prevLocation;
		//dir.GetSafeNormal();
		//FVector p = grabedObject->GetActorLocation() + dir * GetWorld()->DeltaTimeSeconds * throwPower;

		
		
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("%.3f, %.3f, %.3f"), player->motionControllerLeft->GetComponentLocation().X, player->motionControllerLeft->GetComponentLocation().Y, player->motionControllerLeft->GetComponentLocation().Z));
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("%.3f, %.3f, %.3f"), prevLocation.X, prevLocation.Y, prevLocation.Z));
// 		// ���� �
// 		FVector throwDirection = player->motionControllerLeft->GetComponentLocation() - prevLocation;
// 
// 		throwDirection.Normalize();		
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("%.3f, %.3f, %.3f"), throwDirection.X, throwDirection.Y, throwDirection.Z));

		grabedObject->sphereComp->AddImpulse(throwDirection * throwPower);
		
		// ȸ����
		FVector rotAxis = FVector::CrossProduct(prevForward, player->motionControllerLeft->GetForwardVector());
		float angle = FMath::Acos(FVector::DotProduct(prevForward, player->motionControllerLeft->GetForwardVector()));
		angle = FMath::RadiansToDegrees(angle);
		FVector torque = rotAxis * angle;
		grabedObject->sphereComp->AddTorqueInDegrees(torque * torquePower, NAME_None, true);

		DrawDebugLine(GetWorld(), player->motionControllerLeft->GetComponentLocation(), player->motionControllerLeft->GetComponentLocation() + throwDirection * 50, FColor::Red, false, -1, 0, 3);

		// grabedObject ������ ������ nullptr �� �����Ѵ�
		grabedObject = nullptr;
	}
}

void UGraspComponent::DrawGrabRange()
{
	// �� �׸��� (Sphere)
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
			if (!(bIsGrab))
			{
				isEquippingKnife = true;
				knife = GetWorld()->SpawnActor<AKnife>(knifeFactory, player->leftHand->GetComponentTransform());
				
			}
		
		}
		GrapObject(player->leftHand);
		
}