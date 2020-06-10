// Fill out your copyright notice in the Description page of Project Settings.

#include "SnowManAiController.h"
#include "../Player/FoxCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ASnowManAiController::ASnowManAiController()
    : VisibilityRadius(400.f)
    , bFoxInRange(false)
{
    PrimaryActorTick.bCanEverTick = true;

    AISight = CreateDefaultSubobject<UAISenseConfig_Sight>("AISight");
    AISight->SightRadius = VisibilityRadius;
    AISight->LoseSightRadius = (VisibilityRadius + 50.f);
    AISight->PeripheralVisionAngleDegrees = 360.f;
    AISight->SetMaxAge(5.f);
    AISight->DetectionByAffiliation.bDetectEnemies = true;
    AISight->DetectionByAffiliation.bDetectNeutrals = true;
    AISight->DetectionByAffiliation.bDetectFriendlies = true;

    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");

    SetPerceptionComponent(*AIPerception);
}

void ASnowManAiController::BeginPlay()
{
    Super::BeginPlay();

    AIPerception->OnPerceptionUpdated.AddDynamic(this, &ASnowManAiController::OnFoxDetected);

    Fox = Cast<AFoxCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ASnowManAiController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

FRotator ASnowManAiController::GetControlRotation() const
{
    if(Fox)
    {
        return {0.f, Fox->GetActorRotation().Yaw, 0.f};
    }
    return FRotator{};
}

void ASnowManAiController::OnFoxDetected(const TArray<AActor*>& DetectedActors)
{
    if(DetectedActors.Contains(Fox))
    {
        bFoxInRange = true;
    }
}



