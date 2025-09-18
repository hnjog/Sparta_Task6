// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveActor.h"

// Sets default values
AMoveActor::AMoveActor()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 500.0f;
	MoveDistance = 1000.0f;
}

// Called when the game starts or when spawned
void AMoveActor::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	TargetLocation = GetActorLocation();

	if (false == bMoveY)
	{
		TargetLocation += GetActorForwardVector() * MoveDistance;
	}
	else
	{
		TargetLocation += GetActorUpVector() * MoveDistance;
	}
}

void AMoveActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	FVector current = GetActorLocation();
	FVector targetDir = TargetLocation - current;

	SetActorLocation(current + targetDir.GetSafeNormal() * MoveSpeed * deltaTime);

	if (targetDir.Size() < 20.0)
	{
		SetActorLocation(TargetLocation);
		Swap(StartLocation, TargetLocation);
	}
}
