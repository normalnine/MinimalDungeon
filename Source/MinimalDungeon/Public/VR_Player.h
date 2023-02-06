// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <HeadMountedDisplayTypes.h>
#include "VR_Player.generated.h"

UCLASS()
class MINIMALDUNGEON_API AVR_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVR_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, Category = "VR_Setting | Components")
	class UMotionControllerComponent* motionControllerRight;
	UPROPERTY(EditAnywhere, Category = "VR_Setting | Components")
	class UMotionControllerComponent* motionControllerLeft;

	UPROPERTY(EditAnywhere, Category = "VR_Setting | Components")
	class UStaticMeshComponent* rightHand;
	UPROPERTY(EditAnywhere, Category = "VR_Setting | Components")
	class UStaticMeshComponent* leftHand;

	UPROPERTY(EditAnywhere, Category = "VR_Setting | Components")
	class UStaticMeshComponent* sword;

	UPROPERTY(EditAnywhere, Category = "VR_Setting | Components")
	class UCameraComponent* cameraComponent;

	UPROPERTY(EditAnywhere, Category = "VR_Setting | Components")
	class UStaticMeshComponent* HMD;

	UPROPERTY(EditAnywhere, Category = "VR_Setting | MoudulSetting")
	TEnumAsByte <EHMDTrackingOrigin::Type> trackingOrigin;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
	class UInputMappingContext* myMapping;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
	TArray<class UInputAction*> leftInputs;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
	TArray<class UInputAction*> rightInputs;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UMoveComponent* moveComp;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UEquipComponent* equipComp;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UGraspComponent* graspComp;

private:

};
