// Fill out your copyright notice in the Description page of Project Settings.

#include "FoxCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimationAsset.h"

#define TURN_SETTING


template<typename... DataType>
DataHolder<DataType...>::DataHolder(DataType*... Type)
{
    for(auto Index = 0; Index < sizeof...(DataType); Index++)
    {
        this->DataArray.EmplaceAt(Index, Type...);
    }
}


AFoxCharacter::AFoxCharacter()
    : TurnSensitivity(1.f)
    , JumpingForce(1.f)
    , bCanDive(false)
    , bIsDiving(false)
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

    Super::LandedDelegate.AddDynamic(this, &AFoxCharacter::PlayLandingAnimation);

    FIND_OBJECT(RunningSound, USoundCue, /Game/Run.cue);
    FIND_OBJECT(RunningAnimation, UAnimationAsset, /Game/Run.asset);


    FoxSounds = MakeShareable(new DataHolder<USoundCue>(RunningSoundObj.Object));
    FoxAnimations = MakeShareable(new DataHolder<UAnimationAsset>(RunningAnimationObj.Object));

}

AFoxCharacter::~AFoxCharacter()
{
    FoxSounds.Reset();
    FoxAnimations.Reset();
}

void AFoxCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AFoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AFoxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent)

    PlayerInputComponent->BindAxis("MoveForward", this, &AFoxCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFoxCharacter::MoveRight);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFoxCharacter::Jump);
    PlayerInputComponent->BindAction("Dive", IE_Pressed, this, &AFoxCharacter::Dive);
}

void AFoxCharacter::MoveForward(float Value)
{
    if(Controller && Value != 0.0f)
    {
        if(Value < 0.f)
        {
            // TODO add braking animation
        }
        AddMovementInput(GetActorForwardVector(), Value);
        return;
    }
    LOGS(AFoxCharacter::MoveForward called unexpectedly)
}

void AFoxCharacter::MoveRight(float Value)
{
    if(Controller && Value != 0.0f)
    {
        // TODO add turning animation
#ifdef ROTATE_SETTING
        SetActorRotation(FRotator(GetActorRotation().Pitch, (GetActorRotation().Yaw + (Value * TurnSensitivity)), GetActorRotation().Roll));
#elif defined(TURN_SETTING)
        AddMovementInput(GetActorRightVector(), Value);
#endif
        return;
    }
    LOGS(AFoxCharacter::MoveRight called unexpectedly)
}

void AFoxCharacter::Jump()
{
    Super::Jump();
    // TODO add jumping animation

}

void AFoxCharacter::Dive()
{
    // TODO add diving animation
}

void AFoxCharacter::PlayLandingAnimation(const FHitResult& Hit)
{

}




