// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "../AbsolutelyHorrid.h"
#include "GameFramework/PlayerController.h"
#include "HorridPlayerController.generated.h"

class AHorridPawn;

/** Default player controller*/
UCLASS(MinimalAPI)
class AHorridPlayerController final : public APlayerController
{
	GENERATED_BODY()
	
public:

    AHorridPlayerController();

    void SetupInputComponent() override;

    void PlayerTick(float DeltaTime) override;

private:

    void BeginPlay() override;

    UPROPERTY()
    AHorridPawn* HorridPawn;

};
