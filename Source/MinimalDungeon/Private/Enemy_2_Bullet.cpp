// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_2_Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "VR_Player.h"
#include "Enemy_2.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
AEnemy_2_Bullet::AEnemy_2_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	
	RootComponent = collisionComp;
	
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	
	bodyMeshComp->SetupAttachment(collisionComp);

	
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	
	movementComp->SetUpdatedComponent(collisionComp);
	
	movementComp->InitialSpeed = 600;
	
	movementComp->MaxSpeed = 600;

	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_2_Bullet::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AEnemy_2_Bullet::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle ddd;
	GetWorld()->GetTimerManager().SetTimer(ddd,this,&AEnemy_2_Bullet::Die,4.0f,false);

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemy_2::StaticClass());
	me = Cast<AEnemy_2>(actor);
}

// Called every frame
void AEnemy_2_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_2_Bullet::Die()
{
	Destroy();
}

void AEnemy_2_Bullet::OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("OverLap 0"));

	AVR_Player* target = Cast<AVR_Player>(otherActor);
	//UEFSM* enemy = Cast<UEFSM>(otherActor);
	if (target != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OverLap 1"));
		target->ReceiveDamage();

	}
	AEnemy_2* self = Cast<AEnemy_2>(otherActor);
	if (self!= nullptr)
	{
		self->fsm->OnDamageProcess(1);
	}

}
void AEnemy_2_Bullet::returnBack()
{
	
	//// Get the sword's velocity vector
	//FVector SwordVelocity = GetVelocity();

	//// Reflect the bullet's velocity vector along the sword's velocity vector
	//FVector ReflectedVelocity = FMath::GetReflectionVector(GetVelocity(), SwordVelocity);

	//// Set the bullet's new velocity
	//SetActorRotation(ReflectedVelocity.Rotation());
	//SetActorLocation(GetActorLocation() + ReflectedVelocity*500 * GetWorld()->GetDeltaSeconds(), false);

	//SetActorRotation(me->GetActorRotation());

 	FVector dirR = me->GetActorLocation()-GetActorLocation();
 	//dirR.Normalize();
 	FVector p = GetActorLocation() + dirR*10*GetWorld()->DeltaTimeSeconds;
 	FRotator dirRR = dirR.Rotation();
	SetActorRotation(dirRR);
 	SetActorLocation(p);

}