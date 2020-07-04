// Fill out your copyright notice in the Description page of Project Settings.

#include "ScareCrow.h"
#include "SnowManAiController.h"
#include "Components/BoxComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/StaticMeshComponent.h"

AScareCrow::AScareCrow(const FObjectInitializer& ObjectInitializer)
    : ASnowMan(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    MeshComponent->SetupAttachment(RootComponent);
}

void AScareCrow::BeginPlay()
{
    ASnowMan::BeginPlay();
}

void AScareCrow::Tick(float DeltaTime)
{
    ASnowMan::Tick(DeltaTime);
}

