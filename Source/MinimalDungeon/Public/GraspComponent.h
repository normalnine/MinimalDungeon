// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <MotionControllerComponent.h>
#include "GraspComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIMALDUNGEON_API UGraspComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGraspComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 캐스팅을 해야 하기 때문에 처음부터 매개변수를 처음부터 UEnhancedInputComponent 로 받는다.
	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);
		
	UPROPERTY(EditAnywhere, Category = "Inputs")
		class UInputAction* grip_left;

	UPROPERTY(EditAnywhere, Category = "Inputs")
		class UInputAction* grip_right;

	UPROPERTY(EditAnywhere, Category = "Inputs")
		float grapDistance = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
		float throwPower = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Inputs")
		class UInputAction* leftTrigger;

	UPROPERTY(EditAnywhere, Category = "Inputs")
		TSubclassOf<class AKnife> knifeFactory;
	
	UPROPERTY(EditAnywhere, Category = "Inputs")
		float returnSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Inputs")
		float torquePower = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	FVector throwDirection;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	class USoundBase* equipSound;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	class USoundBase* throwSound;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	class USoundBase* graspSound;

	bool bIsGrab = false;
	class APickUpActor* grabedObject;
	class ACellProbActor* cellProb;
	class APickUpFood* food;
	class APickUpBomb* bomb;

	class AVR_Player* player;
	
	bool bPhysicsState = false;
	FVector prevLocation;
	FVector prevForward;
	bool isEquippingKnife = false;
	class AKnife* knife;
	double velocity = 0;
	float currTime  = 0;
	float createTime = 1.0f;
	

	void GripLeftAction(const struct FInputActionValue& value);
	void GripLeftRelease(const struct FInputActionValue& value);
	void GripRightAction(const struct FInputActionValue& value);
	void GripRightRelease(const struct FInputActionValue& value);

	void GrapObject(UStaticMeshComponent* selectHand);
	void ReleaseObject(UStaticMeshComponent* selectHand);
	void DrawGrabRange();
	void EquipKnife();

};
