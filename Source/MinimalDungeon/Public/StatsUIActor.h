// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatsUIActor.generated.h"

UCLASS()
class MINIMALDUNGEON_API AStatsUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatsUIActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* rootComp;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* widgetComp;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* widgetComp_Controller;

	UPROPERTY(EditAnywhere)
	class UStatsUI* statsUI;
};
