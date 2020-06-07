// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AHGameModeBase.h"
#include "AbsolutelyHorrid.h"
#include "Player/HPlayerController.h"
#include "Player/HorridHUD.h"
#include "Player/FoxCharacter.h"

AHGamemodeBase::AHGamemodeBase()
{
    PrimaryActorTick.bCanEverTick = true;
    bPauseable = true;

    DefaultPawnClass = AFoxCharacter::StaticClass();
    PlayerControllerClass = AHPlayerController::StaticClass();
    HUDClass = AHorridHUD::StaticClass();
}

void AHGamemodeBase::Tick(float DeltaTime)
{

}

void AHGamemodeBase::BeginPlay()
{

}



