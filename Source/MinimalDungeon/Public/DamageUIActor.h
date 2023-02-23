// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageUIActor.generated.h"

UCLASS()
class MINIMALDUNGEON_API ADamageUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageUIActor();

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
};
