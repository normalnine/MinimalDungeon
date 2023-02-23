// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy_2.generated.h"

UCLASS()
class MINIMALDUNGEON_API AEnemy_2 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy_2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
		class UEnemy_2_FSM* fsm;

	UPROPERTY(VisibleAnywhere, Category = Extras)
		class UArrowComponent* Arrow1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool dam = false;

};
