// Fill out your copyright notice in the Description page of Project Settings.

#include "HPlayerController.h"


AHPlayerController::AHPlayerController()
{
    AutoReceiveInput = EAutoReceiveInput::Player0;
}

void AHPlayerController::SetupInputComponent()
{
    APlayerController::SetupInputComponent();

}

void AHPlayerController::BeginPlay()
{
    APlayerController::BeginPlay();

}

void AHPlayerController::PlayerTick(float DeltaTime)
{
    APlayerController::PlayerTick(DeltaTime);

}


