// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotateActor.generated.h"

UCLASS()
class TASK6_API ARotateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotateActor();

	virtual void Tick(float deltaTime) override;

	FORCEINLINE void SetRotateSpeed(float rotateSpeed) { RotateSpeed = rotateSpeed; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
	float RotateSpeed;

};
