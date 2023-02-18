// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "potal.generated.h"

UCLASS()
class MINIMALDUNGEON_API Apotal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Apotal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		UPROPERTY(EditAnywhere)
		class UNiagaraComponent* potalEffect;

		UPROPERTY(VisibleAnywhere)
		class USphereComponent* collisionComp;

		UFUNCTION()
			void OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
				int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
