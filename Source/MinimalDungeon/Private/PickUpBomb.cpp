// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpBomb.h"
#include <Components/SphereComponent.h>
#include "Enemy_1.h"
#include "Enemy_2.h"
#include "Enemy_3.h"
#include "Enemy_4.h"
#include "Enemy_1_FSM.h"
#include "Enemy_2_FSM.h"
#include "Enemy_3_FSM.h"
#include "Enemy_4_FSM.h"

void APickUpBomb::BeginPlay()
{
	Super::BeginPlay();
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &APickUpBomb::Overlap);
}

void APickUpBomb::Explode()
{
	FVector Center = GetActorLocation();
	TArray<struct FHitResult> HitResults;
	FCollisionQueryParams params;
	FName ProfileName = "Enemy";
	if (GetWorld()->SweepMultiByProfile(HitResults, Center, Center, FQuat::Identity, ProfileName, FCollisionShape::MakeSphere(exploDistance)))
	{
		DrawDebugSphere(GetWorld(), Center, exploDistance, 30, FColor::Cyan, true, 5, 0, 1);

		for (auto& HitResult : HitResults)
		{
			//float DistanceToHit = (HitActor->GetActorLocation() - GetActorLocation()).Size();
			//float DamageAmount = FMath::Lerp(ExplosionMaxDamage, ExplosionMinDamage, DistanceToHit / ExplosionRadius);
			//FPointDamageEvent DamageEvent(DamageAmount, HitResult, FVector::ZeroVector, UDamageType::StaticClass());
			//HitActor->TakeDamage(DamageAmount, DamageEvent, GetInstigatorController(), this);
			
			AActor* HitActor = HitResult.GetActor();
			if (HitActor)
			{
				AEnemy_1* enemy_1 = Cast<AEnemy_1>(HitActor);
				if (enemy_1 != nullptr)
				{
					enemy_1->fsm->OnDamageProcess();
				}

				AEnemy_2* enemy_2 = Cast<AEnemy_2>(HitActor);
				if (enemy_2 != nullptr)
				{
					enemy_2->fsm->OnDamageProcess();
				}

				AEnemy_3* enemy_3 = Cast<AEnemy_3>(HitActor);
				if (enemy_3 != nullptr)
				{
					enemy_3->fsm->OnDamageProcess();
				}

				AEnemy_4* enemy_4 = Cast<AEnemy_4>(HitActor);
				if (enemy_4 != nullptr)
				{
					enemy_4->fsm->OnDamageProcess();
				}
			
			}
		}
	}

	// 사운드, 이펙트
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	//UGameplayStatics::PlayWorldCameraShake(GetWorld(), ExplosionCameraShake, GetActorLocation(), ExplosionRadius, ExplosionCameraShakeInnerRadius);
	UE_LOG(LogTemp,Warning,TEXT("Bomb!!!"));
	Destroy();
}


void APickUpBomb::Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Explode();
}
