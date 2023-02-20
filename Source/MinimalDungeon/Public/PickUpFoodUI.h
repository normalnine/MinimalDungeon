// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickUpFoodUI.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API UPickUpFoodUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* _foodname;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* _foodeffect;

	UFUNCTION()
	void UpdateInfo(FString foodName, FString foodEffect);
};
