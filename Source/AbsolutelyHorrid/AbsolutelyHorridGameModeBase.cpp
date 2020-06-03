// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AbsolutelyHorridGameModeBase.h"
#include "Player/HorridPlayerController.h"
#include "Player/HorridHUD.h"
#include "Player/HorridPawn.h"

AAbsolutelyHorridGameModeBase::AAbsolutelyHorridGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;
    bPauseable = true;

    PlayerControllerClass = AHorridPlayerController::StaticClass();
    HUDClass = AHorridHUD::StaticClass();
    DefaultPawnClass = AHorridPawn::StaticClass();
}

void AAbsolutelyHorridGameModeBase::Tick(float DeltaTime)
{

}

void AAbsolutelyHorridGameModeBase::BeginPlay()
{

}

