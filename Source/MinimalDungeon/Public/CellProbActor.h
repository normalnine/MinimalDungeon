// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellProbActor.generated.h"


UCLASS()
class MINIMALDUNGEON_API ACellProbActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACellProbActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;


	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* widgetComp;

	UPROPERTY(EditAnywhere)
	class AVR_Player* player;

	UPROPERTY(EditAnywhere)
	class UCellProbUI* cellProbUI;

	UPROPERTY(EditAnywhere)
	class UMD_GameInstance* gameInst;

	UPROPERTY(EditAnywhere)
	int32 price = 0;

	UPROPERTY(EditAnywhere)
	FString probName = "";

	UPROPERTY(EditAnywhere)
	FString probEffect = "";

	UFUNCTION()
	void OverlapHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void PotionEffect();
};
