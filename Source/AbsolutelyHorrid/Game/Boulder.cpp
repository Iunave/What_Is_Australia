// Fill out your copyright notice in the Description page of Project Settings.

#include "Boulder.h"
#include "../AbsolutelyHorrid.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

ABoulder::ABoulder()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
    Mesh->SetMassOverrideInKg(NAME_None, 20000000.f);
    Mesh->SetSimulatePhysics(true);
	Mesh->SetupAttachment(RootComponent);

	OverlapSphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	OverlapSphere->SetupAttachment(Mesh);
}

void ABoulder::BeginPlay()
{
	Super::BeginPlay();

    UGameplayStatics::SpawnSoundAttached(RollingSnow, Mesh, NAME_None, FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, 1.f, 1.f, 0.f, nullptr, nullptr, false);
}

void ABoulder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static const FVector Force{0.f, 200000000.f, 0.f};

	if(Mesh->GetComponentVelocity().X < 575.f && Mesh->GetComponentVelocity().Z < 50.f)
    {
	    Mesh->AddTorqueInRadians(Force);
    }
}

