// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TaskPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 *
 */
UCLASS()
class TASK6_API ATaskPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATaskPlayerController();

	virtual void BeginPlay() override;

	UInputAction* GetMoveAction() const { return MoveAction; }
	UInputAction* GetJumpAction() const { return JumpAction; }
	UInputAction* GetLookAction() const { return LookAction; }
	UInputAction* GetSprintAction() const { return SprintAction; }
	UInputAction* GetRestartAction() const { return RestartAction; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> RestartAction;
};
