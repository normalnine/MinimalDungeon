// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"
#include <Components/SphereComponent.h>
#include <Engine/StaticMesh.h>

AKnife::AKnife()
{
	sphereComp->SetSphereRadius(10);
	meshComp->SetRelativeScale3D(FVector(0.03));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempKnifeMesh(TEXT("/Script/Engine.StaticMesh'/Engine/VREditor/BasicMeshes/SM_Cube_03.SM_Cube_03'"));
	if (tempKnifeMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempKnifeMesh.Object);
	}
}