// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Character.h"
#include "FoxCharacter.generated.h"

class UFoxAnimInstance;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UAnimInstance;
class USoundCue;
class UParticleSystemComponent;
struct FTimerHandle;


UCLASS(Blueprintable)
class ABSOLUTELYHORRID_API AFoxCharacter final : public ACharacter
{
	GENERATED_BODY()

public:

	explicit AFoxCharacter(const FObjectInitializer& ObjectInitializer);
	~AFoxCharacter() override;

    void Tick(float DeltaTime) override;

    void MoveForward(float Value);
    void MoveRight(float Value);

    void Jump() override;

    finline void SetWalkSpeed(const float NewSpeed = 600.f);

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY()
    TArray<USoundCue*> FoxSounds;

protected:

	void BeginPlay() override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void PlayWalkingSound();

    UFUNCTION()
    void OnLanding(const FHitResult& Hit);

    float SetTimerAndSpeed(const float AtSpeed, const float Val, const bool bActivate = true);

    UPROPERTY(VisibleAnywhere, Category=Components)
    UBoxComponent* BoxComponent;

    UPROPERTY(VisibleAnywhere, Category=Components)
    USpringArmComponent* CameraSpringArm;

    UPROPERTY(VisibleAnywhere, Category=Components)
    UCameraComponent* Camera;

    FHitResult* HitResult;
    FCollisionQueryParams* CollisionParams;

    bool bWaitToPlayWalkingSound;
    bool bInvincible;
    bool bFirstSwapTriggered;
    bool bSecondSwapTriggered;

    float WalkSoundDelay;

    UPROPERTY()
    UFoxAnimInstance* FoxAnimInstance;

    FTimerHandle WalkSoundTimerHandle;

    FTimerHandle InvincibilityTimerHandle;

    FTimerHandle ResetWalkSpeed(const float Value, const float Delay);
    
};
