// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

UCLASS()
class TASK6_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnRandActors(UWorld* World, FVector& tempPos);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects")
	FVector TargetPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects")
	double SpawnLimitX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects")
	double SpawnLimitMaxY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects")
	double SpawnLimitMinY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects")
	double SpawnDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects")
	TArray<TSubclassOf<AActor>> SpawnClasses;
};
