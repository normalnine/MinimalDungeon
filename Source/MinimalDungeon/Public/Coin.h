// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

UCLASS()
class MINIMALDUNGEON_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//¿Ü°ü ÄÄÆ÷³ÍÆ®
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* bodyMeshComp;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* collisionComp;


	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void movetotarget();

	void moveStop();

	UPROPERTY(VisibleAnywhere)
		bool canMove = false;

	//Å¸±ê
	UPROPERTY()
		class AVR_Player* player;

};
