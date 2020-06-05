// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "../AbsolutelyHorrid.h"
#include "GameFramework/DefaultPawn.h"
#include "FoxPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;

UCLASS(Blueprintable)
class ABSOLUTELYHORRID_API AFoxPawn final : public ADefaultPawn
{
	GENERATED_BODY()

public:

	AFoxPawn();

    void Tick(float DeltaTime) override;


    inline void MoveForward(float Value) override;
    inline void MoveRight(float Value) override;

    inline void Jump();

private:

	void BeginPlay() override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category=Components)
	USkeletalMeshComponent* SkeletalMesh;

    UPROPERTY(VisibleAnywhere, Category=Components)
    USpringArmComponent* CameraSpringArm;

    UPROPERTY(VisibleAnywhere, Category=Components)
    UCameraComponent* Camera;

    float TurnSensitivity;
    float JumpingForce;
    double FoxVelocity;

    FVector LastTickLocation;

};
