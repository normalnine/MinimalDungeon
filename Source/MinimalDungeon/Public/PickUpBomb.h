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
	APickUpBomb();

	UPROPERTY(EditAnywhere)
	float exploDistance = 100;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* skeletalComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf <class AActor> exploEffect;

	UPROPERTY(EditAnywhere)
	class USoundBase* exploSound;
	
	UFUNCTION()
	void Explode();

	UFUNCTION()
	void Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
