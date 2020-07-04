// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Animation/AnimInstance.h"
#include "FoxAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EFoxState : uint8
{
    Idle,

    Running,

    Jumping,

    InAir,

    Landing
};

/** class to handle animation for fox */
UCLASS()
class ABSOLUTELYHORRID_API UFoxAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

friend class AFoxCharacter;
	
public:

    UFoxAnimInstance();

    finline void SetFoxState(const EFoxState State)
    {
        if(State != FoxState)
        {
            FoxState = State;
        }
    }

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EFoxState FoxState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float FoxSpeed;

};
