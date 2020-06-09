// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "SnowMan.generated.h"

class USoundCue;
class UCapsuleComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class ABSOLUTELYHORRID_API ASnowMan : public ADefaultPawn
{
	GENERATED_BODY()

public:

	ASnowMan();

    virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	USoundCue* MoveSound;

    UPROPERTY()
    USoundCue* BreakSound;

    UPROPERTY()
    USoundCue* Laughing;

};
