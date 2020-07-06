// Fill out your copyright notice in the Description page of Project Settings.

#include "FoxCharacter.h"
#include "../Game/Boulder.h"
#include "../Game/SnowMan.h"
#include "../Game/ScareCrow.h"
#include "../Game/Triggers/SoundTrigger1.h"
#include "../Game/Triggers/SoundTrigger2.h"
#include "../Game/Triggers/WinTrigger.h"
#include "../AbsolutelyHorrid.h"
#include "FoxAnimInstance.h"

#include "TimerManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

#define ENEMY_HIT(Sound) \
UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation()); \
GetCharacterMovement()->StopMovementImmediately(); \
SetWalkSpeed(200.f); \
ResetWalkSpeed(600.f, 1.f); \
bInvincible = true; \
GetWorld()->GetTimerManager().SetTimer(InvincibilityTimerHandle, [&](){bInvincible = false;}, 1.5f, false);

#define ENEMY_HIT_CAUGHT(Sound, SoundTwo) \
ENEMY_HIT(Sound) \
UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundTwo, GetActorLocation());


AFoxCharacter::AFoxCharacter(const FObjectInitializer& ObjectInitializer)
    : bWaitToPlayWalkingSound(false)
    , bInvincible(false)
    , bFirstSwapTriggered(false)
    , bSecondSwapTriggered(false)
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

	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxAcceleration = 600.f;

    FIND_OBJECT(StepsGrass, USoundCue, /Game/Assets/Sounds/SFX/Steps_Grass);
    FIND_OBJECT(GrassSnow, USoundCue, /Game/Assets/Sounds/SFX/Steps_SnowGround);
    FIND_OBJECT(StepsSnow, USoundCue, /Game/Assets/Sounds/SFX/Steps_Snow);
    FIND_OBJECT(Sqeak, USoundCue, /Game/Assets/Sounds/SFX/FoxScream);
    FIND_OBJECT(Jumping, USoundCue, /Game/Assets/Sounds/SFX/FoxJump);
    FIND_OBJECT(Landing, USoundCue, /Game/Assets/Sounds/SFX/FoxLand);
    FIND_OBJECT(Squasched, USoundCue, /Game/Assets/Sounds/SFX/FoxDeath);
    FIND_OBJECT(WinGame, USoundCue, /Game/Assets/Sounds/SFX/UI_WinGame);

    FoxSounds.Add(StepsGrassObj.Object);
    FoxSounds.Add(GrassSnowObj.Object);
    FoxSounds.Add(StepsSnowObj.Object);
    FoxSounds.Add(SqeakObj.Object);
    FoxSounds.Add(JumpingObj.Object);
    FoxSounds.Add(LandingObj.Object);
    FoxSounds.Add(SquaschedObj.Object);
    FoxSounds.Add(WinGameObj.Object);

    HitResult = new FHitResult();
    CollisionParams = new FCollisionQueryParams();
}

AFoxCharacter::~AFoxCharacter()
{
    delete HitResult;
    delete CollisionParams;

    for(uint8 Index{0}; Index < FoxSounds.Num(); Index++)
    {
        FoxSounds.RemoveAt(Index);
    }
}

void AFoxCharacter::BeginPlay()
{
	Super::BeginPlay();

    LandedDelegate.AddDynamic(this, &AFoxCharacter::OnLanding);

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFoxCharacter::OnBeginOverlap);
    BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFoxCharacter::OnEndOverlap);

    verify(FoxAnimInstance = Cast<UFoxAnimInstance>(GetMesh()->GetAnimInstance()));

    FoxSounds.Swap(0, 2);

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

            if(GetWorld()->LineTraceSingleByChannel(*HitResult, StartLocation, EndLocation, ECC_Visibility, *CollisionParams))
            {
                GetCharacterMovement()->AddImpulse((HitResult->ImpactNormal) * 5000.f);
            }
        };
        DoLineTrace(FVector(1.f, 0.f, 0.f), 80.f);
        DoLineTrace(FVector(1.f, -0.5f, 0.f), 35.f);
        DoLineTrace(FVector(-1.f, 0.75f, 0.f), 25.f);
        DoLineTrace(FVector(-1.f, 0.f, 0.f), 80.f);
        DoLineTrace(FVector(-1.f, -0.75f, 0.f), 25.f);
        DoLineTrace(FVector(1.f, 0.5f, 0.f), 35.f);
    }

    if(GetMovementComponent()->Velocity.X > 0.f)
    {
        FoxAnimInstance->FoxSpeed = GetMovementComponent()->Velocity.X + (GetMovementComponent()->Velocity.Y > 0.f ? GetMovementComponent()->Velocity.Y : GetMovementComponent()->Velocity.Y * -1);
    }
    else if(GetMovementComponent()->Velocity.X < 0.f)
    {
        FoxAnimInstance->FoxSpeed = GetMovementComponent()->Velocity.X * -1 + (GetMovementComponent()->Velocity.Y > 0.f ? GetMovementComponent()->Velocity.Y / 2 : (GetMovementComponent()->Velocity.Y * -1) / 2);
    }

    SetActorRotation(FRotator(GetActorRotation().Pitch, ((GetMovementComponent()->Velocity.Y / GetCharacterMovement()->MaxWalkSpeed) * 25.f), GetActorRotation().Roll), ETeleportType::TeleportPhysics);
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

        if(Value < 0.f)
        {
            AddMovementInput(FVector::ForwardVector, SetTimerAndSpeed(0.5f, Value));
            return;
        }
        AddMovementInput(FVector::ForwardVector, SetTimerAndSpeed(1.f, Value));
        return;
    }
}

void AFoxCharacter::MoveRight(float Value)
{
    if(Controller && Value != 0.0f && !GetCharacterMovement()->IsFalling())
    {
        FoxAnimInstance->SetFoxState(EFoxState::Running);

        if(GetVelocity().X < -10.f)
        {
            AddMovementInput(FVector::RightVector, SetTimerAndSpeed(0.5f, Value, FMath::IsNearlyZero(GetVelocity().X, 5.f)));
            return;
        }
        AddMovementInput(FVector::RightVector, SetTimerAndSpeed(1.f, Value, FMath::IsNearlyZero(GetVelocity().X, 5.f)));
        return;
    }
}

void AFoxCharacter::Jump()
{
    if(FoxAnimInstance->FoxState != EFoxState::Jumping)
    {
        Super::Jump();

        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FoxSounds[4], GetActorLocation());

        FoxAnimInstance->SetFoxState(EFoxState::Jumping);
        FoxAnimInstance->ParallelUpdateAnimation();
    }
}

void AFoxCharacter::SetWalkSpeed(const float WalkSpeed)
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFoxCharacter::OnLanding(const FHitResult& Hit)
{
    FoxAnimInstance->SetFoxState(EFoxState::Landing);

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), FoxSounds[5], Hit.ImpactPoint);
}

void AFoxCharacter::PlayWalkingSound()
{
    if(!GetCharacterMovement()->IsFalling() && !GetVelocity().IsNearlyZero(10.f))
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FoxSounds[0], GetActorLocation());
    }
    bWaitToPlayWalkingSound = false;
}

float AFoxCharacter::SetTimerAndSpeed(const float AtSpeed, const float Val, const bool bActivate)
{
    if(!bWaitToPlayWalkingSound && bActivate)
    {
        GetWorld()->GetTimerManager().SetTimer(WalkSoundTimerHandle, this, &AFoxCharacter::PlayWalkingSound, (WalkSoundDelay / AtSpeed));
        bWaitToPlayWalkingSound = true;
    }
    return Val * AtSpeed;
}

void AFoxCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(OtherActor->IsA<ABoulder>())
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FoxSounds[6], GetActorLocation());
        FTimerHandle DestroyWorldHandle;
        GetWorldTimerManager().SetTimer(DestroyWorldHandle, [&](){UGameplayStatics::OpenLevel(GetWorld(), FName("Level"));}, 0.65f, false);
    }
    else if(!bInvincible)
    {
        if(ASnowMan* SnowMan = Cast<ASnowMan>(OtherActor))
        {
            if(GetVelocity().X >= 550.f)
            {
                ENEMY_HIT(SnowMan->Break)
                SnowMan->Destroy();
            }
            else{
                ENEMY_HIT_CAUGHT(FoxSounds[3], SnowMan->Laugh)
            }
        }
        else if(AScareCrow* ScareCrow = Cast<AScareCrow>(OtherActor))
        {
            if(GetVelocity().X >= 550.f)
            {
                ENEMY_HIT(ScareCrow->Break)
                ScareCrow->Destroy();
            }
            else{
                ENEMY_HIT_CAUGHT(FoxSounds[3], ScareCrow->Laugh)
            }
        }
    }
    if(OtherActor->IsA<ASoundTrigger1>() && !bFirstSwapTriggered)
    {
        FoxSounds.Swap(0, 1);
        bFirstSwapTriggered = true;
    }
    else if(OtherActor->IsA<ASoundTrigger2>() && !bSecondSwapTriggered)
    {
        FoxSounds.Swap(0, 2);
        bSecondSwapTriggered = true;
    }
    else if(OtherActor->IsA<AWinTrigger>())
    {
        UGameplayStatics::PlaySound2D(GetWorld(), FoxSounds[7]);
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

