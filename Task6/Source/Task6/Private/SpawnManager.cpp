// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "DropActor.h"
#include "MoveActor.h"
#include "RotateActor.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
	TargetPos = FVector(-2730.0, -110.0, 462.0);
	SpawnLimitX = 3200.0;
	SpawnLimitMaxY = 3300.0;
	SpawnLimitMinY = -3300.0;
	SpawnDistance = 600.0;
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	int32 SpawnClassSize = SpawnClasses.Num();
	if (SpawnClassSize == 0)
		return;

	UWorld* World = GetWorld();
	if (World == nullptr)
		return;

	for (double XPos = TargetPos.X; XPos <= SpawnLimitX; XPos += SpawnDistance)
	{
		for (double YPos = TargetPos.Y; YPos <= SpawnLimitMaxY; YPos += SpawnDistance)
		{
			FVector TempPos = FVector(XPos, YPos, TargetPos.Z);

			SpawnRandActors(World, TempPos);
		}

		for (double YPos = TargetPos.Y - SpawnDistance; YPos > SpawnLimitMinY; YPos -= SpawnDistance)
		{
			FVector TempPos = FVector(XPos, YPos, TargetPos.Z);

			SpawnRandActors(World, TempPos);
		}
	}
}

void ASpawnManager::SpawnRandActors(UWorld* World, FVector& tempPos)
{
	int32 RandClass = FMath::RandRange(0, SpawnClasses.Num() - 1);

	TSubclassOf<AActor> SelectedClass = SpawnClasses[RandClass];

	AActor* SpawnedActor = World->SpawnActor(SelectedClass, &tempPos);

	if (ADropActor* DropActor = Cast<ADropActor>(SpawnedActor))
	{
		float RandTime = FMath::RandRange(2.0f, 5.0f);
		DropActor->SetDropTime(RandTime);
	}
	else if (ARotateActor* RotateActor = Cast<ARotateActor>(SpawnedActor))
	{
		float RandRotateSpeed = FMath::RandRange(25.0f, 75.0f);
		RotateActor->SetRotateSpeed(RandRotateSpeed);
	}
	else if (AMoveActor* MoveActor = Cast<AMoveActor>(SpawnedActor))
	{
		float RandMoveSpeed = FMath::RandRange(250.0f, 450.0f);
		MoveActor->SetMoveSpeed(RandMoveSpeed);

		float RandMoveDistance = FMath::RandRange(500.0f, 1000.0f);
		MoveActor->SetMoveDistance(RandMoveSpeed);
	}
}


