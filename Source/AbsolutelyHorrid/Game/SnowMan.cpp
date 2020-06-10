// Fill out your copyright notice in the Description page of Project Settings.

#include "SnowMan.h"
#include "../AbsolutelyHorrid.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"

ASnowMan::ASnowMan()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("SnowMan");
	StaticMesh->SetupAttachment(RootComponent);

}

void ASnowMan::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASnowMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

