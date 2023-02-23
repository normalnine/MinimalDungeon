/// Fill out your copyright notice in the Description page of Project Settings.


#include "BuyComponent.h"
#include "VR_Player.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "CellProbActor.h"
#include <Components/TextRenderComponent.h>
#include "MD_GameInstance.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UBuyComponent::UBuyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBuyComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AVR_Player>(GetOwner());

	gameInst = Cast<UMD_GameInstance>(GetWorld()->GetGameInstance());
}


// Called every frame
void UBuyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBuyComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(buttonX, ETriggerEvent::Started, this, &UBuyComponent::PressButtonX);
	PlayerInputComponent->BindAction(buttonA, ETriggerEvent::Started, this, &UBuyComponent::PressButtonA);
}



void UBuyComponent::PressButtonX()
{
	BuyCellProb(player->leftHand);
}

void UBuyComponent::PressButtonA()
{
	BuyCellProb(player->rightHand);
}

void UBuyComponent::BuyCellProb(UStaticMeshComponent* selectHand)
{
	FVector Center = selectHand->GetComponentLocation();
	FHitResult hitInfo;
	FName profileName = FName(TEXT("CellProb"));
	FCollisionQueryParams params;
	params.AddIgnoredActor(player);
	//DrawDebugSphere(GetWorld(), Center, 10.0f, 30, FColor::Cyan, false, -1, 0, 1);

	if (GetWorld()->SweepSingleByProfile(hitInfo, Center, Center, FQuat::Identity, profileName, FCollisionShape::MakeSphere(10), params))
	{
		cellProb = Cast<ACellProbActor>(hitInfo.GetActor());

		if (IsValid(cellProb))
		{
			if (gameInst->coin >= cellProb->price)
			{
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0, 0.3f, 0.5, FLinearColor::Blue);
				UGameplayStatics::PlaySound2D(GetWorld(), buffSound);
				cellProb->PotionEffect();
				gameInst->coin -= cellProb->price;
				GetWorld()->DestroyActor(cellProb);
				player->textCompCoinNum->SetText(FText::AsNumber(gameInst->coin));
			}
		}
	}
}

