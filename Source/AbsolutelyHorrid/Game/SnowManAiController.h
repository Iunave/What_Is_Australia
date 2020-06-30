// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "AIController.h"
#include "SnowManAiController.generated.h"

class AFoxCharacter;
class APatrolPoint;
class ASnowMan;

/** Controller for snowman & scarecrow */
UCLASS()
class ABSOLUTELYHORRID_API ASnowManAiController : public AAIController
{
	GENERATED_BODY()

	friend class ASnowMan;
	friend class AScareCrow;

public:

    ASnowManAiController();

	virtual FRotator GetControlRotation() const override;

    virtual void Tick(float DeltaTime) override;

    virtual void OnPossess(APawn* Pawn) override;

protected:

    virtual void BeginPlay() override;

    UPROPERTY()
    AFoxCharacter* Fox;

    bool bFoxInRange;

};
