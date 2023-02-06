// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "VR_Player.h"
#include "CollisionDebugDrawingPublic.h"
#include "Math/RotationMatrix.h"
#include <Components/CapsuleComponent.h>
#include "TeleportRingActor.h"
#include "NiagaraComponent.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ������ �÷��̾� Ŭ������ ĳ���Ѵ�.
	player = Cast<AVR_Player>(GetOwner());

	// ���� ���� Ŭ������ ĳ���Ѵ�.
	currentWorld = GetWorld();
	
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (blsShowLine)
	{
		DrawMoveLine();
	}
}

void UMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	//PlayerInputComponent->BindAction(leftThumbstick, ETriggerEvent::Triggered, this, &UMoveComponent::Move);
	PlayerInputComponent->BindAction(rightThumbstick, ETriggerEvent::Triggered, this, &UMoveComponent::RotateAxis);
	PlayerInputComponent->BindAction(leftThumbstick, ETriggerEvent::Triggered, this, &UMoveComponent::ShowLine);
	PlayerInputComponent->BindAction(leftThumbstick, ETriggerEvent::Completed, this, &UMoveComponent::HideLine);
}

void UMoveComponent::Move(const struct FInputActionValue& value)
{
	FVector2D val = value.Get<FVector2D>();
	FVector direction = FVector(val.Y, val.X, 0);

	player->AddMovementInput(direction.GetSafeNormal(), 1, false);
}

void UMoveComponent::RotateAxis(const struct FInputActionValue& value)
{
	float val = value.Get<float>();
	// axis ���� �̿��ؼ� ĳ����(��Ʈ�ѷ�)�� ȸ���Ѵ�.
	player->AddControllerYawInput(val);

}

void UMoveComponent::DrawMoveLine()
{
	// �̵��� ��� ȭ�鿡 �׸���.

	// 1. �̵��� ��ġ ����(TArray<Fvector>)�� ���Ѵ�.


	float timeInterval = 0.02f;
	int32 timeSegment = 100;

// 	FVector dir = (FVector::ForwardVector + FVector::UpVector);
// 	dir = player->leftHand->GetComponentTransform().TransformVector(dir);

	//�޼� �𵨸��� ���� ��ǥ�� ���缭 forward ����� up ������ �ٽ� ����Ѵ�.
	FVector handForward = FRotationMatrix(player->leftHand->GetComponentRotation()).GetUnitAxis(EAxis::Y) * -0.5;
	FVector handUP = FRotationMatrix(player->leftHand->GetComponentRotation()).GetUnitAxis(EAxis::X);
	FVector dir = handForward + handUP;

	lineLoc.Empty();

	for (int32 i = 0; i < timeSegment; i++)
	{
		// �ҿ� �ð�
		float timeTaken = timeInterval * i;
		// p = p0 +vt
		FVector prediction = player->leftHand->GetComponentLocation() + dir.GetSafeNormal() * power * timeTaken;
		// p.Z -= 0.5 * g *t^2
		prediction.Z += 0.5f * currentWorld->GetDefaultGravityZ() * timeTaken * timeTaken;

		// 1-2. 0.1�ʾ� 0~50��° ������ ����ؼ� �迭�� �ִ´�.	
		lineLoc.Add(prediction);

		// ����Ʈ���̽��� �������� üũ�Ѵ�
		FHitResult hitInfo;
		if (i > 0)
		{
			if (currentWorld->LineTraceSingleByChannel(hitInfo, lineLoc[i - 1], lineLoc[i], ECC_Visibility))
			{
				// �ε��� ������ ������ ��ǥ�� �ְ� �ݺ����� ���� �����Ѵ�.
				lineLoc.Add(hitInfo.ImpactPoint);
				break;
			}
		}
	}

	// 2. �տ��� ���� ��ġ ���� ������ ������ �׸��� �׸���(DrawLine)	
	for (int32 i = 0; i < lineLoc.Num() - 1; i++)
	{
		DrawDebugLine(GetWorld(), lineLoc[i], lineLoc[i + 1], FColor::Green, false, -1, 0, 2);
	}

	// �ڷ���Ʈ �� ����Ʈ�� ������ ���� ��ġ�� ��ġ�Ѵ�.
	if (spawned_fx == nullptr)
	{
		// ����Ʈ�� �����Ѵ�.
		spawned_fx = currentWorld->SpawnActor<ATeleportRingActor>(teleport_fx, lineLoc[lineLoc.Num() - 1], FRotator::ZeroRotator);
	}
	else
	{
		// �Ⱥ��̰� ó���� ����Ʈ�� �ٽ� ���̰� �Ѵ�.
		spawned_fx->niagara_fx->SetVisibility(true);
		spawned_fx->SetActorLocation(lineLoc[lineLoc.Num() - 1]);
	}

}

void UMoveComponent::Teleport()
{
	// ����Ų �������� ���� �̵�(��� ��ǥ ����) �Ѵ�.
	FVector targetLoc = lineLoc[lineLoc.Num() - 1];
	targetLoc.Z += player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	player->SetActorLocation(targetLoc, false, nullptr, ETeleportType::TeleportPhysics);

	if (spawned_fx != nullptr)
	{
		spawned_fx->niagara_fx->SetVisibility(false);
	}
}

void UMoveComponent::ShowLine()
{
	blsShowLine = true;
}
void UMoveComponent::HideLine()
{
	blsShowLine = false;
	Teleport();
}