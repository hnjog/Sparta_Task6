// Fill out your copyright notice in the Description page of Project Settings.

#include "TaskPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "EnhancedInputComponent.h"
#include "TaskPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATaskPlayer::ATaskPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->TargetOffset.Set(0.0, 0.0, 150.0);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
	CameraComp->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Resource/Robot_scout_R_21/Mesh/SK_Robot_scout_R21.SK_Robot_scout_R21"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -90.0));
		GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

		// Anim Class를 찾아 세팅
		// Class를 찾는것이기에 마지막에 _C 를 붙인다!
		ConstructorHelpers::FClassFinder<UAnimInstance> MeshAnimAsset(TEXT("/Game/Resource/Robot_scout_R_21/Demo/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));
		if (MeshAnimAsset.Succeeded())
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(MeshAnimAsset.Class);
		}
	}

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ATaskPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATaskPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATaskPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced InputComponent로 캐스팅
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
		if (ATaskPlayerController* PlayerController = Cast<ATaskPlayerController>(GetController()))
		{
			if (UInputAction* MoveAction = PlayerController->GetMoveAction())
			{
				// IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
				EnhancedInput->BindAction(
					MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ATaskPlayer::Move
				);
			}

			if (UInputAction* JumpAction = PlayerController->GetJumpAction())
			{
				// IA_Jump 액션 키를 "키를 누르고 있는 동안" StartJump() 호출
				EnhancedInput->BindAction(
					JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ATaskPlayer::StartJump
				);

				// IA_Jump 액션 키에서 "손을 뗀 순간" StopJump() 호출
				EnhancedInput->BindAction(
					JumpAction,
					ETriggerEvent::Completed,
					this,
					&ATaskPlayer::StopJump
				);
			}

			if (UInputAction* LookAction = PlayerController->GetLookAction())
			{
				// IA_Look 액션 마우스가 "움직일 때" Look() 호출
				EnhancedInput->BindAction(
					LookAction,
					ETriggerEvent::Triggered,
					this,
					&ATaskPlayer::Look
				);
			}

			if (UInputAction* SprintAction = PlayerController->GetSprintAction())
			{
				// IA_Sprint 액션 키를 "누르고 있는 동안" StartSprint() 호출
				EnhancedInput->BindAction(
					SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ATaskPlayer::StartSprint
				);
				// IA_Sprint 액션 키에서 "손을 뗀 순간" StopSprint() 호출
				EnhancedInput->BindAction(
					SprintAction,
					ETriggerEvent::Completed,
					this,
					&ATaskPlayer::StopSprint
				);
			}
		}
	}
}

void ATaskPlayer::Move(const FInputActionValue& value)
{
	if (Controller == nullptr)
		return;

	const FVector2D MoveValue = value.Get<FVector2D>();

	const float YValue = GetControlRotation().Yaw;
	const FRotator YawRot(0.f, YValue, 0.f);

	if (FMath::IsNearlyZero(MoveValue.X) == false)
	{
		//FVector Right = RootComponent->GetRightVector() 
		const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		AddMovementInput(Forward, MoveValue.X);
	}

	if (FMath::IsNearlyZero(MoveValue.Y) == false)
	{
		const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		AddMovementInput(Right, MoveValue.Y);
	}
}

void ATaskPlayer::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>() == true)
	{
		Jump();
	}
}

void ATaskPlayer::StopJump(const FInputActionValue& value)
{
	if (value.Get<bool>() == false)
	{
		StopJumping();
	}
}

void ATaskPlayer::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	// X는 좌우 회전 (Yaw), Y는 상하 회전 (Pitch)
	
	// 좌우 회전
	AddControllerYawInput(LookInput.X);
	// 상하 회전
	AddControllerPitchInput(LookInput.Y);
}

void ATaskPlayer::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ATaskPlayer::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}
