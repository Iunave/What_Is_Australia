// Fill out your copyright notice in the Description page of Project Settings.

#include "Boulder.h"
#include "../AbsolutelyHorrid.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Triggers/SoundTrigger2.h"

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

	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoulder::OnBeginOverlap);

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

void ABoulder::OnBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if(OtherActor->IsA<ASoundTrigger2>())
    {
        RollingSnow = nullptr;
        UGameplayStatics::SpawnSoundAttached(RollingGrass, Mesh, NAME_None, FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, 1.f, 1.f, 0.f, nullptr, nullptr, false);
    }
}

