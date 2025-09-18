// Fill out your copyright notice in the Description page of Project Settings.


#include "DropActor.h"
#include "TaskPlayer.h"

// Sets default values
ADropActor::ADropActor()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComp;
	DropTime = 5.0f;
}

// Called when the game starts or when spawned
void ADropActor::BeginPlay()
{
	Super::BeginPlay();

	bOnce = false;
	StaticMeshComp->OnComponentHit.AddDynamic(this, &ADropActor::OnHit);
}

void ADropActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bOnce)
		return;

	if (Cast<ATaskPlayer>(OtherActor) == nullptr)
		return;

	bOnce = true;
	GetWorldTimerManager().SetTimer(DropTimerHandle, this, &ADropActor::Drop, DropTime, false);
}

void ADropActor::Drop()
{
	StaticMeshComp->SetSimulatePhysics(true);
}