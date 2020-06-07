// Fill out your copyright notice in the Description page of Project Settings.

#include "FoxCharacter.h"
#include "../AbsolutelyHorrid.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"

#define TURN_SETTING


AFoxCharacter::AFoxCharacter(const FObjectInitializer& ObjectInitializer)
    : TurnSensitivity(1.f)
    , JumpingForce(1.f)
    , bCanDive(false)
    , bIsDiving(false)
    , bWaitToPlayWalkingSound(false)
    , WalkSoundDelay(0.2f)
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

    FIND_OBJECT(StepsSnow, USoundCue, /Game/Assets/Sounds/SFX/Steps_Snow);
    FIND_OBJECT(StepsGrass, USoundCue,/Game/Assets/Sounds/SFX/Steps_Grass);
    FIND_OBJECT(Dazed, USoundCue,/Game/Assets/Sounds/SFX/Fox_Dazed);

    FoxSounds = MakeShareable(new DataHolder<USoundCue>(StepsGrassObj.Object, StepsSnowObj.Object, DazedObj.Object));

    FoxAnimations = MakeShareable(new DataHolder<UAnimBlueprint>());

}

AFoxCharacter::~AFoxCharacter() noexcept
{
    FoxSounds.Reset();
    FoxAnimations.Reset();
}

void AFoxCharacter::BeginPlay()
{
	Super::BeginPlay();

    Super::LandedDelegate.AddDynamic(this, &AFoxCharacter::PlayLandingAnimation);

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFoxCharacter::OnBeginOverlap);
    BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFoxCharacter::OnEndOverlap);
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
    if(Controller && Value != 0.0f && !GetCharacterMovement()->IsFalling())
    {
        auto SetTimerForWalkingSFX = [&](const float AtSpeed) -> void
        {
            if(!bWaitToPlayWalkingSound)
            {
                GetWorld()->GetTimerManager().SetTimer(WalkingSoundTimerHandle, this, &AFoxCharacter::PlaySound, (WalkSoundDelay / AtSpeed));
                bWaitToPlayWalkingSound = true;
            }
        };

        if(Value < 0.f)
        {
            static constexpr float Speed {0.5f};

            SetTimerForWalkingSFX(Speed);
            AddMovementInput(GetActorForwardVector(), (Value * Speed));
            return;
        }
        SetTimerForWalkingSFX(1.f);

        AddMovementInput(GetActorForwardVector(), Value);
        return;
    }

    LOGS(AFoxCharacter::MoveForward called unexpectedly)
}

void AFoxCharacter::MoveRight(float Value)
{
    if(Controller && Value != 0.0f && !GetCharacterMovement()->IsFalling())
    {
        if(!bWaitToPlayWalkingSound && FMath::IsNearlyZero(GetVelocity().X, 5.f))
        {
            GetWorld()->GetTimerManager().SetTimer(WalkingSoundTimerHandle, this, &AFoxCharacter::PlaySound, WalkSoundDelay);
            bWaitToPlayWalkingSound = true;
        }
        // TODO add turning animation
#ifdef ROTATE_SETTING

        SetActorRotation(FRotator(GetActorRotation().Pitch, (GetActorRotation().Yaw + (Value * TurnSensitivity)), GetActorRotation().Roll));

#elif defined(TURN_SETTING)

        if(GetVelocity().X < -10.f)
        {
            AddMovementInput(GetActorRightVector(), (Value * 0.5f));
            return;
        }
        AddMovementInput(GetActorRightVector(), Value);
#endif
        return;
    }
    LOGS(AFoxCharacter::MoveRight called unexpectedly)
}

void AFoxCharacter::Jump()
{
    abort();
}

void AFoxCharacter::Dive()
{
    // TODO add diving animation
}

void AFoxCharacter::PlayLandingAnimation(const FHitResult& Hit)
{
    // TODO add landing animation
}

void AFoxCharacter::PlaySound()
{
    if(!GetCharacterMovement()->IsFalling() && !GetVelocity().IsNearlyZero(10.f) && FoxSounds.IsValid())
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FoxSounds->DataArray[0], GetActorLocation());
    }
    bWaitToPlayWalkingSound = false;
}


void AFoxCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(!OtherActor->IsA<AFoxCharacter>())
    {

    }
}

void AFoxCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(!OtherActor->IsA<AFoxCharacter>())
    {

    }
}