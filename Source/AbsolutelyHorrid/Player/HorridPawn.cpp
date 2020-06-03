// Fill out your copyright notice in the Description page of Project Settings.

#include "HorridPawn.h"


AHorridPawn::AHorridPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

}

void AHorridPawn::BeginPlay()
{
	APawn::BeginPlay();
	
}

void AHorridPawn::Tick(float DeltaTime)
{
    APawn::Tick(DeltaTime);

}

void AHorridPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{

}


