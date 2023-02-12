// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "Knife.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API AKnife : public APickUpActor
{
	GENERATED_BODY()

public:
	AKnife();
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	class UGraspComponent* graspComp;

	UPROPERTY()
	class AVR_Player* player;

	UPROPERTY()
	bool bKnifeStudded = false;
private:
	UFUNCTION()
	void KnifeAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
