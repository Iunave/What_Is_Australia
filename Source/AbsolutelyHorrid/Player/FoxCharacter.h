// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "../AbsolutelyHorrid.h"
#include "GameFramework/Character.h"
#include "FoxCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AHPlayerController;

UCLASS(Blueprintable)
class ABSOLUTELYHORRID_API AFoxCharacter final : public ACharacter
{
	GENERATED_BODY()

public:

	AFoxCharacter();

    void Tick(float DeltaTime) override;


    inline void MoveForward(float Value);
    inline void MoveRight(float Value);

private:

	void BeginPlay() override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, Category="Components")
    USpringArmComponent* CameraSpringArm;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UCameraComponent* Camera;

    UPROPERTY()
    AHPlayerController* PlayerControllerPtr{};

    float TurnSensitivity;

};
