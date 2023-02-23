// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuyComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINIMALDUNGEON_API UBuyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBuyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere, Category = "Inputs")
		class UInputAction* buttonX;

	UPROPERTY(EditAnywhere, Category = "Inputs")
		class UInputAction* buttonA;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	class USoundBase* buffSound;

	class AVR_Player* player;

	class ACellProbActor* cellProb;

	class UMD_GameInstance* gameInst;

	void PressButtonX();
	void PressButtonA();
	void BuyCellProb(UStaticMeshComponent* selectHand);
};
