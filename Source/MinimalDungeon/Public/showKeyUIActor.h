// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "showKeyUIActor.generated.h"

UCLASS()
class MINIMALDUNGEON_API AshowKeyUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AshowKeyUIActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* wcomp;

	UPROPERTY(EditAnywhere)
		class UshowKeyUI* key;
};
