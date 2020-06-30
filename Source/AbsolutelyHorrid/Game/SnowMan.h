// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Character.h"
#include "../Player/FoxCharacter.h"
#include "SnowMan.generated.h"

class USoundCue;
class UBoxComponent;
class UPawnSensingComponent;

/** Enemy Snowman */
UCLASS(Blueprintable)
class ABSOLUTELYHORRID_API ASnowMan : public ACharacter
{
	GENERATED_BODY()

public:

    ASnowMan();

    ASnowMan(FVTableHelper& Helper)
    : Super(Helper)
    , VisibilityRadius(400.f)
    {
    }

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    UBoxComponent* Box;

protected:

    friend void AFoxCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = AI)
    UPawnSensingComponent* PawnSense;

private:

    UFUNCTION()
    void OnActorCaught(APawn* Pawn);

    const float VisibilityRadius;

    UPROPERTY(EditDefaultsOnly)
    USoundCue* MoveSound;

    UPROPERTY(EditDefaultsOnly)
    USoundCue* BreakSound;

    UPROPERTY(EditDefaultsOnly)
    USoundCue* Laughing;

};
