// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CellProbUI.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API UCellProbUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* _price;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* _probname;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* _probeffect;

	UFUNCTION()
	void UpdateInfo(int32 price, FString name, FString effect);
};
