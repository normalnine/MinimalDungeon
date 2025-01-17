// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIMALDUNGEON_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 캐스팅을 해야 하기 때문에 처음부터 매개변수를 처음부터 UEnhancedInputComponent 로 받는다.
	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere, Category = "Inputs")
	class UInputAction* leftThumbstick;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	class UInputAction* rightThumbstick;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	class UInputAction* leftTrigger;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	float power = 500;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TSubclassOf<class ATeleportRingActor> teleport_fx;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float TeleportTime = 1.0f;

private:
	class AVR_Player* player;
	TArray<FVector> lineLoc;
	bool blsShowLine = false;
	class UWorld* currentWorld;
	class ATeleportRingActor* spawned_fx;

	void Move(const struct FInputActionValue& value);
	void RotateAxis(const struct FInputActionValue& value);
	void DrawMoveLine();
	void Teleport();
	void ShowLine();
	void HideLine();
		
};
