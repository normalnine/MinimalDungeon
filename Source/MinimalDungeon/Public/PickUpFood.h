// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "PickUpFood.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API APickUpFood : public APickUpActor
{
	GENERATED_BODY()

protected:
	void BeginPlay();

public:
	APickUpFood();

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* widgetComp;

	UPROPERTY(EditAnywhere)
	FString foodName = "Pear";
	
	UPROPERTY(EditAnywhere)
	FString foodEffect = "HP +1";

	UPROPERTY(EditAnywhere)
	class UPickUpFoodUI* foodUI;
};
