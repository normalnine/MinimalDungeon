// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "Knife.generated.h"

/**
 * 
 */
UCLASS()
class MINIMALDUNGEON_API AKnife : public APickUpActor
{
	GENERATED_BODY()

public:
	AKnife();
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	class UGraspComponent* graspComp;

	UPROPERTY()
	class AVR_Player* player;



	UPROPERTY()
	class UMD_GameInstance* gameInst;

	UPROPERTY()
	bool bKnifeStudded = false;

	UPROPERTY(BlueprintReadWrite)
	int32 knifeAttackDmg = 0;

	UPROPERTY(EditAnywhere)
	TSubclassOf <class AActor> hitEffect;
private:
	UFUNCTION()
	void KnifeAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		TSubclassOf <class AActor> damageUIActorFactory;

	UPROPERTY(EditAnywhere)
		class ADamageUIActor* damageUIActor;

	UPROPERTY(EditAnywhere)
		class UDamageUI* damageUI;

	void SpawnDamageUI(AActor* OtherActor);
};
