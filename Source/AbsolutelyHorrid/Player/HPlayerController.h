// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "../AbsolutelyHorrid.h"
#include "GameFramework/PlayerController.h"
#include "HPlayerController.generated.h"

class AFoxCharacter;

/** Default player controller*/
UCLASS(MinimalAPI)
class AHPlayerController final : public APlayerController
{
	GENERATED_BODY()
	
public:

    AHPlayerController();

    void SetupInputComponent() override;

    void PlayerTick(float DeltaTime) override;

private:

    void BeginPlay() override;

    UPROPERTY()
    AFoxCharacter* FoxCharacterPtr;

};
