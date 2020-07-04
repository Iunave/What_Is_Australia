// Fill out your copyright notice in the Description page of Project Settings.

#include "SnowMan.h"
#include "../AbsolutelyHorrid.h"
#include "SnowManAiController.h"
#include "Components/BoxComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Boulder.h"


ASnowMan::ASnowMan(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , VisibilityRadius(500.f)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5f;

	Box = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	Box->SetupAttachment(RootComponent);

	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>("PawnSense");
	PawnSense->SightRadius = VisibilityRadius;
}

void ASnowMan::BeginPlay()
{
	Super::BeginPlay();

	PawnSense->OnSeePawn.AddDynamic(this, &ASnowMan::OnActorCaught);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ASnowMan::OnBeginOverlap);
}

void ASnowMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetMovementComponent()->Velocity.X != 0.f || GetMovementComponent()->Velocity.Y != 0.f)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), Move, GetActorLocation());
    }
}

void ASnowMan::OnBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if(OtherActor->IsA<ABoulder>())
    {
        this->Destroy();
    }
}

void ASnowMan::OnActorCaught(APawn* Pawn)
{
    if(ASnowManAiController* AiController = GetController<ASnowManAiController>())
    {
        AiController->bFoxInRange = Cast<TRemovePointer<decltype(AiController->Fox)>::Type>(Pawn) != nullptr;
    }
}

