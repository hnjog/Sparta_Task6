// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameMode.h"
#include "TaskPlayer.h"

ATaskGameMode::ATaskGameMode()
{
	DefaultPawnClass = ATaskPlayer::StaticClass();
}
