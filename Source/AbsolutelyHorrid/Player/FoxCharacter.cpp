// Fill out your copyright notice in the Description page of Project Settings.

#include "FoxCharacter.h"
#include "HPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AFoxCharacter::AFoxCharacter()
    : TurnSensitivity(10.f)
{
	PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpingArmComponent");
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;
}

void AFoxCharacter::BeginPlay()
{
	ACharacter::BeginPlay();

    PlayerControllerPtr = Cast<AHPlayerController>(Controller);
}

void AFoxCharacter::Tick(float DeltaTime)
{
    ACharacter::Tick(DeltaTime);

}

void AFoxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent)

    PlayerInputComponent->BindAxis("MoveForward", this, &AFoxCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFoxCharacter::MoveRight);
}

void AFoxCharacter::MoveForward(float Value)
{
    if(Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
        return;
    }
    LOGS(AFoxCharacter::MoveForward called unexpectedly)
}

void AFoxCharacter::MoveRight(float Value)
{
    if(Value != 0.0f)
    {
        SetActorRotation(FRotator(GetActorRotation().Pitch, (GetActorRotation().Yaw + (Value * TurnSensitivity)), GetActorRotation().Roll));
        return;
    }
    LOGS(AFoxCharacter::MoveRight called unexpectedly)
}



