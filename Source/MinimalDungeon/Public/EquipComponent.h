// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIMALDUNGEON_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ĳ������ �ؾ� �ϱ� ������ ó������ �Ű������� ó������ UEnhancedInputComponent �� �޴´�.
	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere, Category = "Inputs")
	class UInputAction* buttonY;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	class USoundBase* equipSound;

	bool isEquippingSword = true;

	class AVR_Player* player;
	


	void EquipSword();
	
};

