// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClimbComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIMALDUNGEON_API UClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClimbComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* grab_r;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* grab_l;


	bool bClimbing_right = false;
	bool bClimbing_left = false;
	bool CanClimb = false;

	class AVR_Player* player;
	FVector climb_start;
	class UMotionControllerComponent* currentController;

	void StartClimb(bool isLeft);
	void EndClimb();
	void GrabLeft();
	void GrabRight();
	void ReleaseLeft();
	void ReleaseRight();
	bool CheckHolder(class UMotionControllerComponent* controller);
};
