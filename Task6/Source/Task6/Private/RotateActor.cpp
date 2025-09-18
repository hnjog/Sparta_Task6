// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateActor.h"

// Sets default values
ARotateActor::ARotateActor()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(RootComponent);
	RotateSpeed = 50.f;

	PrimaryActorTick.bCanEverTick = true;
}


void ARotateActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	AddActorLocalRotation(FRotator(0.0f, deltaTime * RotateSpeed, 0.0f));
}


