// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	// ĳ������ �ؾ� �ϱ� ������ ó������ �Ű������� ó������ UEnhancedInputComponent �� �޴´�.
	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);
		
	UPROPERTY(EditAnywhere, Category = "Inputs")
		class UInputAction* grip_left;

	UPROPERTY(EditAnywhere, Category = "Inputs")
		class UInputAction* grip_right;

	UPROPERTY(EditAnywhere, Category = "Inputs")
		float grapDistance = 30.0f;

private:
	class AVR_Player* player;
	void GripLeftAction(const struct FInputActionValue& value);
	void GripRightAction(const struct FInputActionValue& value);
	void GrapObject(UStaticMeshComponent* selectHand);


};
