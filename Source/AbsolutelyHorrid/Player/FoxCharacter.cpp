// Fill out your copyright notice in the Description page of Project Settings.

#include "FoxCharacter.h"
#include "../Game/Boulder.h"
#include "../Game/SnowMan.h"
#include "../Game/ScareCrow.h"
#include "../AbsolutelyHorrid.h"
#include "FoxAnimInstance.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"


AFoxCharacter::AFoxCharacter(const FObjectInitializer& ObjectInitializer)
    : bWaitToPlayWalkingSound(false)
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

    FIND_OBJECT(StepsGrass, USoundCue, /Game/Assets/Sounds/SFX/Steps_Grass);
    FIND_OBJECT(StepsSnow, USoundCue, /Game/Assets/Sounds/SFX/Steps_Snow);
    FIND_OBJECT(Sqeak, USoundCue, /Game/Assets/Sounds/SFX/FoxScream);
    FIND_OBJECT(Jumping, USoundCue, /Game/Assets/Sounds/SFX/FoxJump);
    FIND_OBJECT(Landing, USoundCue, /Game/Assets/Sounds/SFX/FoxLand);
    FIND_OBJECT(Squasched, USoundCue, /Game/Assets/Sounds/SFX/FoxDeath);
    FIND_OBJECT(Dazed, USoundCue,/Game/Assets/Sounds/SFX/Fox_Dazed);
    FIND_OBJECT(GameOver, USoundCue, /Game/Assets/Sounds/SFX/GameOver);

    FoxSounds.Emplace(StepsGrassObj.Object);
    FoxSounds.Emplace(StepsSnowObj.Object);
    FoxSounds.Emplace(SqeakObj.Object);
    FoxSounds.Emplace(JumpingObj.Object);
    FoxSounds.Emplace(LandingObj.Object);
    FoxSounds.Emplace(SquaschedObj.Object);
    FoxSounds.Emplace(DazedObj.Object);
    FoxSounds.Emplace(GameOverObj.Object);

    HitResult = new FHitResult();
    CollisionParams = new FCollisionQueryParams();
}

AFoxCharacter::~AFoxCharacter()
{
    delete HitResult;
    delete CollisionParams;
}

void AFoxCharacter::BeginPlay()
{
	Super::BeginPlay();

    LandedDelegate.AddDynamic(this, &AFoxCharacter::OnLanding);

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFoxCharacter::OnBeginOverlap);
    BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFoxCharacter::OnEndOverlap);

    ParticleSystem->ActivateSystem();

    verify(FoxAnimInstance = Cast<UFoxAnimInstance>(GetMesh()->GetAnimInstance()));

    FoxSounds.Swap(0, 1);

    CollisionParams->AddIgnoredActor(this);
}

void AFoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    {
        const FVector StartLocation {GetActorLocation() + FVector(0.f, 0.f, 80.f)};

        FVector EndLocation;

        auto DoLineTrace = [&](const FVector&& TowardsVector, const float ForDistance) -> void
        {
            EndLocation = (TowardsVector * ForDistance) + StartLocation;

            if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartLocation, EndLocation, ECC_Visibility, *CollisionParams))
            {
                GetCharacterMovement()->AddRadialImpulse(HitResult->ImpactPoint, 50.f, 500.f, ERadialImpulseFalloff::RIF_Constant, true);
            }
        };

        DoLineTrace(FVector(1.f, 0.f, 0.f), 80.f);
        DoLineTrace(FVector(1.f, -0.5f, 0.f), 35.f);
        DoLineTrace(FVector(-1.f, 0.75f, 0.f), 25.f);
        DoLineTrace(FVector(-1.f, 0.f, 0.f), 80.f);
        DoLineTrace(FVector(-1.f, -0.75f, 0.f), 25.f);
        DoLineTrace(FVector(1.f, 0.5f, 0.f), 35.f);
    }

    if(GetMovementComponent()->Velocity.X > 0)
    {
        FoxAnimInstance->FoxSpeed = GetMovementComponent()->Velocity.X;
    }
    else if(GetMovementComponent()->Velocity.X < 0)
    {
        FoxAnimInstance->FoxSpeed = (GetMovementComponent()->Velocity.X * -1);
    }
    else if(!GetMovementComponent()->IsFalling())
    {
        FoxAnimInstance->SetFoxState(EFoxState::Idle);
    }
}

void AFoxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent)

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFoxCharacter::Jump);

    PlayerInputComponent->BindAxis("MoveForward", this, &AFoxCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFoxCharacter::MoveRight);
}

void AFoxCharacter::MoveForward(float Value)
{
    if(Controller && Value != 0.0f && !GetCharacterMovement()->IsFalling())
    {
        FoxAnimInstance->SetFoxState(EFoxState::Running);

        auto SetTimerAndSpeed = [&](const float AtSpeed) -> float
        {
            if(!bWaitToPlayWalkingSound)
            {
                GetWorld()->GetTimerManager().SetTimer(WalkSoundTimer, this, &AFoxCharacter::PlayWalkingSound, (WalkSoundDelay / AtSpeed));
                bWaitToPlayWalkingSound = true;
            }
            return Value * AtSpeed;
        };

        if(Value < 0.f)
        {
            AddMovementInput(GetActorForwardVector(), SetTimerAndSpeed(0.5f));
            return;
        }
        AddMovementInput(GetActorForwardVector(), SetTimerAndSpeed(1.f));
        return;
    }
    LOGS(AFoxCharacter::MoveForward called unexpectedly)
}

void AFoxCharacter::MoveRight(float Value)
{
    if(Controller && Value != 0.0f && !GetCharacterMovement()->IsFalling())
    {
        FoxAnimInstance->SetFoxState(EFoxState::Running);

        if(!bWaitToPlayWalkingSound && FMath::IsNearlyZero(GetVelocity().X, 5.f))
        {
            GetWorld()->GetTimerManager().SetTimer(WalkSoundTimer, this, &AFoxCharacter::PlayWalkingSound, WalkSoundDelay);
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
    if(FoxAnimInstance->FoxState == EFoxState::Running || FoxAnimInstance->FoxState == EFoxState::Idle)
    {
        Super::Jump();

        //FoxAnimInstance->Montage_Play(FoxAnimInstance->FoxAnimations->DataArray[0]);

        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FoxSounds[3], GetActorLocation());
        ParticleSystem->DeactivateSystem();

        FoxAnimInstance->SetFoxState(EFoxState::Jumping);
    }
}

void AFoxCharacter::SetWalkSpeed(const float WalkSpeed)
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFoxCharacter::OnLanding(const FHitResult& Hit)
{
    FoxAnimInstance->SetFoxState(EFoxState::Landing);

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), FoxSounds[4], Hit.ImpactPoint);
    ParticleSystem->ActivateSystem();
}

void AFoxCharacter::PlayWalkingSound()
{
    if(!GetCharacterMovement()->IsFalling() && !GetVelocity().IsNearlyZero(10.f))
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FoxSounds[0], GetActorLocation());
    }
    bWaitToPlayWalkingSound = false;
}

#define ON_ENEMY_HIT(Sound) \
UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation()); \
GetCharacterMovement()->StopMovementImmediately(); \
SetWalkSpeed(200.f); \
ResetWalkSpeed(600.f, 1.f); \

void AFoxCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(OtherActor->IsA<ABoulder>())
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FoxSounds[2], GetActorLocation());
    }
    else if(ASnowMan* SnowMan = Cast<ASnowMan>(OtherActor))
    {
        if(GetVelocity().X >= 500.f)
        {
            ON_ENEMY_HIT(SnowMan->BreakSound)
            SnowMan->Destroy();
            return;
        }
        ON_ENEMY_HIT(FoxSounds[2])
    }
    else if(AScareCrow* ScareCrow = Cast<AScareCrow>(OtherActor))
    {
        if(GetVelocity().X >= 500.f)
        {
            ON_ENEMY_HIT(nullptr)
            ScareCrow->Destroy();
            return;
        }
        ON_ENEMY_HIT(FoxSounds[2])
    }
}

void AFoxCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

FTimerHandle AFoxCharacter::ResetWalkSpeed(const float Value, const float Delay)
{
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &AFoxCharacter::SetWalkSpeed, Value), Delay, false);
    return TimerHandle;
}

