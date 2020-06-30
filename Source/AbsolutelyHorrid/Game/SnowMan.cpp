// Fill out your copyright notice in the Description page of Project Settings.

#include "SnowMan.h"
#include "../AbsolutelyHorrid.h"
#include "SnowManAiController.h"
#include "Components/BoxComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/PawnMovementComponent.h"


ASnowMan::ASnowMan()
    : VisibilityRadius(500.f)
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	Box->SetupAttachment(RootComponent);

	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>("PawnSense");
	PawnSense->SightRadius = VisibilityRadius;
}

void ASnowMan::BeginPlay()
{
	Super::BeginPlay();

	PawnSense->OnSeePawn.AddDynamic(this, &ASnowMan::OnActorCaught);
}

void ASnowMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnowMan::OnActorCaught(APawn* Pawn)
{
    if(ASnowManAiController* AiController = GetController<ASnowManAiController>())
    {
        AiController->bFoxInRange = Cast<TRemovePointer<decltype(AiController->Fox)>::Type>(Pawn) != nullptr;
    }
}

