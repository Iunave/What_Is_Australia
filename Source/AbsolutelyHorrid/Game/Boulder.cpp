// Fill out your copyright notice in the Description page of Project Settings.

#include "Boulder.h"
#include "Components/StaticMeshComponent.h"

ABoulder::ABoulder()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	Mesh->SetupAttachment(RootComponent);

}

void ABoulder::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABoulder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

