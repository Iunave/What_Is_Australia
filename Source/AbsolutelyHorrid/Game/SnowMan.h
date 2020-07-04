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

    explicit ASnowMan(const FObjectInitializer& ObjectInitializer);
	virtual ~ASnowMan() override = default;

    ASnowMan(FVTableHelper& Helper)
    : Super(Helper)
    , VisibilityRadius(400.f)
    {
    }

    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

    friend void AFoxCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere)
    UBoxComponent* Box;

	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = AI)
    UPawnSensingComponent* PawnSense;

    UFUNCTION()
    virtual void OnActorCaught(APawn* Pawn);

    const float VisibilityRadius;

    UPROPERTY(EditDefaultsOnly)
    USoundCue* Move;

    UPROPERTY(EditDefaultsOnly)
    USoundCue* Break;

    UPROPERTY(EditDefaultsOnly)
    USoundCue* Laugh;

public:

    finline UBoxComponent* GetBoxComponent() const {return Box;}

};
