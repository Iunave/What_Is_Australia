// Fill out your copyright notice in the Description page of Project Settings.

#include "WinTrigger.h"
#include "Components/BoxComponent.h"


AWinTrigger::AWinTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>("HitBox");
    Box->SetupAttachment(RootComponent);
}

void AWinTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void AWinTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

