// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatsUI.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API UStatsUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* _swordDmg;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* _swordCrit;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* _knifeDmg;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* _knifeCrit;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* _evasion;

	UPROPERTY(EditAnywhere)
	class UMD_GameInstance* gameInst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* _giveup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* _gomain;

	UFUNCTION()
	void UpdateStats();

	UFUNCTION()
	void GiveUp();

	UFUNCTION()
	void GoMain();
};
