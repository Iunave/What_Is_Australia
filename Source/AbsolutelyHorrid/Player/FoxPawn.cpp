// Fill out your copyright notice in the Description page of Project Settings.

#include "FoxPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"


AFoxPawn::AFoxPawn()
    : TurnSensitivity(1.f)
    , JumpingForce(1.f)
    , LastTickLocation(0.f, 0.f, 0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SkeletalMesh->SetupAttachment(RootComponent);

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void AFoxPawn::BeginPlay()
{
	ADefaultPawn::BeginPlay();

}

void AFoxPawn::Tick(float DeltaTime)
{
    ADefaultPawn::Tick(DeltaTime);

    if(LastTickLocation != GetActorLocation())
    {
        const FVector ThisTickLocation = GetActorLocation();

        FoxVelocity = (static_cast<double>(ThisTickLocation.X) - static_cast<double>(LastTickLocation.X))
                    + (static_cast<double>(ThisTickLocation.Y) - static_cast<double>(LastTickLocation.Y));

        LastTickLocation = ThisTickLocation;
    }
}

void AFoxPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent)

    PlayerInputComponent->BindAxis("MoveForward", this, &AFoxPawn::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFoxPawn::MoveRight);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFoxPawn::Jump);
}

void AFoxPawn::MoveForward(float Value)
{
    if(Controller && Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
        return;
    }
    LOGS(AFoxCharacter::MoveForward called unexpectedly)
}

void AFoxPawn::MoveRight(float Value)
{
    if(Controller && Value != 0.0f)
    {
        SetActorRotation(FRotator(GetActorRotation().Pitch, (GetActorRotation().Yaw + (Value * TurnSensitivity)), GetActorRotation().Roll));
        return;
    }
    LOGS(AFoxCharacter::MoveRight called unexpectedly)
}

void AFoxPawn::Jump()
{
    if(Controller)
    {
        AddMovementInput(GetActorUpVector(), JumpingForce);
    }
}



