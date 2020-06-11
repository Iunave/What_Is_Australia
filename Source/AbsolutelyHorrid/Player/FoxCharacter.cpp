// Fill out your copyright notice in the Description page of Project Settings.

#include "FoxCharacter.h"
#include "../AbsolutelyHorrid.h"
#include "../Game/Boulder.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "../Game/SnowMan.h"
#include "../Game/ScareCrow.h"
#include "GameFramework/CharacterMovementComponent.h"


AFoxCharacter::AFoxCharacter(const FObjectInitializer& ObjectInitializer)
    : bCanDive(false)
    , bIsDiving(false)
    , bCanPlayJumpSound(true)
    , bWaitToPlayWalkingSound(false)
    , WalkSoundDelay(0.2f)
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);

    ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
    ParticleSystem->SetupAttachment(RootComponent);
    ParticleSystem->SetAutoActivate(false);

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

    /*
    AnimationInstance->MontageInstances.Emplace(nullptr);
	AnimationInstance->bUseMultiThreadedAnimationUpdate = true;
	AnimationInstance->Montage_Play();
    */

    FIND_OBJECT(StepsSnow, USoundCue, /Game/Assets/Sounds/SFX/Steps_Snow);
    FIND_OBJECT(StepsGrass, USoundCue,/Game/Assets/Sounds/SFX/Steps_Grass);
    FIND_OBJECT(Sqeak, USoundCue, /Game/Assets/Sounds/SFX/FoxScream);
    FIND_OBJECT(Jumping, USoundCue, /Game/Assets/Sounds/SFX/FoxJump);
    FIND_OBJECT(Landing, USoundCue, /Game/Assets/Sounds/SFX/FoxLand);
    FIND_OBJECT(Squasched, USoundCue, /Game/Assets/Sounds/SFX/FoxDeath);
    FIND_OBJECT(Dazed, USoundCue,/Game/Assets/Sounds/SFX/Fox_Dazed);
    FIND_OBJECT(GameOver, USoundCue, /Game/Assets/Sounds/SFX/GameOver);

    FoxSounds = MakeShareable(new DataHolder<USoundCue>
                (
                    StepsGrassObj.Object,
                    StepsSnowObj.Object,
                    SqeakObj.Object,
                    JumpingObj.Object,
                    LandingObj.Object,
                    SquaschedObj.Object,
                    DazedObj.Object,
                    GameOverObj.Object
                ));

    FoxAnimations = MakeShareable(new DataHolder<UAnimInstance>());

}

AFoxCharacter::~AFoxCharacter() noexcept
{
    FoxSounds.Reset();
    FoxAnimations.Reset();
}

void AFoxCharacter::BeginPlay()
{
	Super::BeginPlay();

    Super::LandedDelegate.AddDynamic(this, &AFoxCharacter::OnLanding);

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFoxCharacter::OnBeginOverlap);
    BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFoxCharacter::OnEndOverlap);

    ParticleSystem->ActivateSystem();

    ThisWorld = GetWorld();

    FoxSounds->DataArray.Swap(0, 1);
}

void AFoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const FVector StartLocation {GetActorLocation() + FVector(0.f, 0.f, 80.f)};

    auto DoLineTrace = [&](const FVector&& TowardsVector, const float ForDistance) -> void
    {
        auto* HitResult = new FHitResult();
        auto* CollisionParams = new FCollisionQueryParams();

        const FVector EndLocation {(TowardsVector * ForDistance) + StartLocation};

        CollisionParams->AddIgnoredActor(this);

        if(ThisWorld->LineTraceSingleByChannel(*HitResult, StartLocation, EndLocation, ECC_Visibility, *CollisionParams))
        {
            GetCharacterMovement()->AddRadialImpulse(HitResult->ImpactPoint, 100.f, 500.f, ERadialImpulseFalloff::RIF_Constant, true);
        }
        delete HitResult;
        delete CollisionParams;
    };

    DoLineTrace(FVector(1.f, 0.f, 0.f), 80.f);
    DoLineTrace(FVector(1.f, -0.5f, 0.f), 35.f);
    DoLineTrace(FVector(-1.f, 0.75f, 0.f), 25.f);
    DoLineTrace(FVector(-1.f, 0.f, 0.f), 80.f);
    DoLineTrace(FVector(-1.f, -0.75f, 0.f), 25.f);
    DoLineTrace(FVector(1.f, 0.5f, 0.f), 35.f);

}

void AFoxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent)

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFoxCharacter::Jump);
    PlayerInputComponent->BindAction("Dive", IE_Pressed, this, &AFoxCharacter::Dive);

    PlayerInputComponent->BindAxis("MoveForward", this, &AFoxCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFoxCharacter::MoveRight);
}

void AFoxCharacter::MoveForward(float Value)
{
    if(Controller && Value != 0.0f && !GetCharacterMovement()->IsFalling())
    {
        auto SetTimerForWalkingSFX = [&](const float AtSpeed) -> void
        {
            if(!bWaitToPlayWalkingSound)
            {
                ThisWorld->GetTimerManager().SetTimer(WalkSoundTimer, this, &AFoxCharacter::PlayWalkingSound, (WalkSoundDelay / AtSpeed));
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
            ThisWorld->GetTimerManager().SetTimer(WalkSoundTimer, this, &AFoxCharacter::PlayWalkingSound, WalkSoundDelay);
            bWaitToPlayWalkingSound = true;
        }
        if(GetVelocity().X < -10.f)
        {
            AddMovementInput(GetActorRightVector(), (Value * 0.5f));
            return;
        }
        AddMovementInput(GetActorRightVector(), Value);
        return;
    }
    LOGS(AFoxCharacter::MoveRight called unexpectedly)
}

void AFoxCharacter::Jump()
{
    Super::Jump();
    if(bCanPlayJumpSound)
    {
        UGameplayStatics::PlaySoundAtLocation(ThisWorld, FoxSounds->DataArray[3], GetActorLocation());
        ParticleSystem->DeactivateSystem();
        bCanPlayJumpSound = false;
    }
}

void AFoxCharacter::Dive()
{
    // TODO add diving animation
}

void AFoxCharacter::SetWalkSpeed(const float WalkSpeed)
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFoxCharacter::OnLanding(const FHitResult& Hit)
{
    // TODO add landing animation
    UGameplayStatics::PlaySoundAtLocation(ThisWorld, FoxSounds->DataArray[4], GetActorLocation());
    ParticleSystem->ActivateSystem();
    bCanPlayJumpSound = true;
}

void AFoxCharacter::PlayWalkingSound()
{
    if(!GetCharacterMovement()->IsFalling() && !GetVelocity().IsNearlyZero(10.f) && FoxSounds.IsValid())
    {
        UGameplayStatics::PlaySoundAtLocation(ThisWorld, FoxSounds->DataArray[0], GetActorLocation());
    }
    bWaitToPlayWalkingSound = false;
}


void AFoxCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(auto* BoulderPtr = Cast<ABoulder>(OtherActor))
    {
        UGameplayStatics::PlaySoundAtLocation(ThisWorld, FoxSounds->DataArray[2], GetActorLocation());
    }
    else if(OtherActor->IsA<ASnowMan>() || OtherActor->IsA<AScareCrow>())
    {
        UGameplayStatics::PlaySoundAtLocation(ThisWorld, FoxSounds->DataArray[2], GetActorLocation());

        GetCharacterMovement()->StopMovementImmediately();
        SetWalkSpeed(200.f);
        ResetWalkSpeed(600.f, 1.f);
    }
    else if(GetActorLocation().X >= 2800.f)
    {
        FoxSounds->DataArray.Swap(0, 1);
    }
}

void AFoxCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

FTimerHandle AFoxCharacter::ResetWalkSpeed(const float Value, const float Delay)
{
    FTimerHandle TimerHandle;
    ThisWorld->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &AFoxCharacter::SetWalkSpeed, Value), Delay, false);
    return TimerHandle;
}