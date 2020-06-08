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
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"

#if !UE_BUILD_SHIPPING
#include "Engine.h"
#endif


AFoxCharacter::AFoxCharacter(const FObjectInitializer& ObjectInitializer)
    : JumpingForce(1.f)
    , bCanDive(false)
    , bIsDiving(false)
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

    FIND_OBJECT(StepsSnow, USoundCue, /Game/Assets/Sounds/SFX/Steps_Snow);
    FIND_OBJECT(StepsGrass, USoundCue,/Game/Assets/Sounds/SFX/Steps_Grass);
    FIND_OBJECT(Dazed, USoundCue,/Game/Assets/Sounds/SFX/Fox_Dazed);

    FoxSounds = MakeShareable(new DataHolder<USoundCue>(StepsSnowObj.Object, StepsGrassObj.Object, DazedObj.Object));

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

    ParticleSystem->ActivateSystem();
}

void AFoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const FVector StartLocation {GetActorLocation() + FVector(0.f, 0.f, 80.f)};

    auto DoLineTrace = [&](const FVector&& TowardsVector, const float ForDistance) -> void
    {
        auto *HitResult = new FHitResult();
        auto *CollisionParams = new FCollisionQueryParams();

        const FVector EndLocation {(TowardsVector * ForDistance) + StartLocation};

        CollisionParams->AddIgnoredActor(this);

        if(GetWorld()->LineTraceSingleByChannel(*HitResult, StartLocation, EndLocation, ECC_Visibility, *CollisionParams))
        {
            GetCharacterMovement()->StopMovementImmediately();
        }
        #if !UE_BUILD_SHIPPING
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Emerald, false, 0.1f, 0, 2.5f);
        #endif
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