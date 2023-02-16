// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "PickUpBomb.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API APickUpBomb : public APickUpActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	float exploDistance = 30;
	
	UFUNCTION()
	void Explode();

	UFUNCTION()
	void Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
