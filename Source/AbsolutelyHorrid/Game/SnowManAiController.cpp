// Fill out your copyright notice in the Description page of Project Settings.

#include "SnowManAiController.h"
#include "../Player/FoxCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


ASnowManAiController::ASnowManAiController()
    : bFoxInRange(false)
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASnowManAiController::BeginPlay()
{
    Super::BeginPlay();

    verify(Fox = Cast<AFoxCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
}

void ASnowManAiController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bFoxInRange)
    {
        MoveToLocation(Fox->GetActorLocation(), 10.f, true, false);
    }
}

void ASnowManAiController::OnPossess(APawn* Pawn)
{
    Super::OnPossess(Pawn);
}

FRotator ASnowManAiController::GetControlRotation() const
{
    if(Fox)
    {
        return {0.f, Fox->GetActorRotation().Yaw, 0.f};
    }
    return FRotator(ForceInitToZero);
}



