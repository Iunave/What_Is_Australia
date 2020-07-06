// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundTrigger1.h"
#include "Components/BoxComponent.h"

ASoundTrigger1::ASoundTrigger1()
{
	PrimaryActorTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>("HitBox");
    Box->SetupAttachment(RootComponent);
}

void ASoundTrigger1::BeginPlay()
{
	Super::BeginPlay();
}

void ASoundTrigger1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

