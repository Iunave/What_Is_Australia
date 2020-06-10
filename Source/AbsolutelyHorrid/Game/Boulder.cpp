// Fill out your copyright notice in the Description page of Project Settings.

#include "Boulder.h"
#include "../AbsolutelyHorrid.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ABoulder::ABoulder()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
    Mesh->SetSimulatePhysics(true);
    Mesh->SetMassOverrideInKg(NAME_None, 100.f);
	Mesh->SetupAttachment(RootComponent);

	OverlapSphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	OverlapSphere->SetupAttachment(RootComponent);

	FIND_OBJECT(RollingSnow, USoundCue, /Game/Assets/Sounds/SFX/Rock_WithSnow);
    FIND_OBJECT(RollingGrass, USoundCue, /Game/Assets/Sounds/SFX/Rock_NoSnow);

    RollingSounds = MakeShareable(new DataHolder<USoundCue>(RollingSnowObj.Object, RollingGrassObj.Object));
}

void ABoulder::BeginPlay()
{
	Super::BeginPlay();

    UGameplayStatics::SpawnSoundAttached(RollingSounds->DataArray[0], RootComponent, NAME_None, FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, 1.f, 1.f, 0.f, nullptr, nullptr, false);
}

void ABoulder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetVelocity().X >= 600.f)
    {
	    Mesh->AddForce(FVector(1000.f, 0.f, 0.f));
	    return;
    }
	PrimaryActorTick.bCanEverTick = false;
}

