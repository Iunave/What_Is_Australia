// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SnowManAiController.generated.h"

class AFoxCharacter;
class UAISenseConfig_Sight;
class UAIPerceptionComponent;

/** Controller for snowman */
UCLASS()
class ABSOLUTELYHORRID_API ASnowManAiController : public AAIController
{
	GENERATED_BODY()
	
public:

    ASnowManAiController();

	virtual FRotator GetControlRotation() const override;

    UFUNCTION()
	void OnFoxDetected(const TArray<AActor*>& DetectedActors);

    virtual void Tick(float DeltaTime) override;

protected:

    virtual void BeginPlay() override;

    float VisibilityRadius;

    UPROPERTY()
    AFoxCharacter* Fox;

    UPROPERTY(EditAnywhere)
    UAISenseConfig_Sight* AISight;

    UPROPERTY(EditAnywhere)
    UAIPerceptionComponent* AIPerception;

    bool bFoxInRange;

};
