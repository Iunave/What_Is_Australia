// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundTrigger2.h"
#include "Components/BoxComponent.h"

ASoundTrigger2::ASoundTrigger2()
{
	PrimaryActorTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>("HitBox");
    Box->SetupAttachment(RootComponent);
}

void ASoundTrigger2::BeginPlay()
{
	Super::BeginPlay();
}

void ASoundTrigger2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

