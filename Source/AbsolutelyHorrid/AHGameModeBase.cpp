// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AHGameModeBase.h"
#include "Player/HPlayerController.h"
#include "Player/HorridHUD.h"
#include "Player/FoxCharacter.h"

AHGamemodeBase::AHGamemodeBase()
{
    PrimaryActorTick.bCanEverTick = true;
    bPauseable = true;

    PlayerControllerClass = AHPlayerController::StaticClass();
    HUDClass = AHorridHUD::StaticClass();
    DefaultPawnClass = AFoxCharacter::StaticClass();
}

void AHGamemodeBase::Tick(float DeltaTime)
{

}

void AHGamemodeBase::BeginPlay()
{

}



