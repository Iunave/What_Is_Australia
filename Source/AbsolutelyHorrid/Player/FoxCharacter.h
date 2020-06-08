// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Character.h"
#include "FoxCharacter.generated.h"

template<typename DataType> struct DataHolder;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UAnimInstance;
class USoundCue;
class UParticleSystemComponent;
struct FTimerHandle;
class FTimerDynamicDelegate;


UCLASS(Blueprintable)
class ABSOLUTELYHORRID_API AFoxCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AFoxCharacter(const FObjectInitializer& ObjectInitializer);
	inline ~AFoxCharacter() noexcept override;

    void Tick(float DeltaTime) override;

    void MoveForward(float Value);

    void MoveRight(float Value);

    FORCENOINLINE void Jump() override;

    FORCENOINLINE void Dive();

    FORCEINLINE void SetWalkSpeed(const float NewSpeed = 600.f);

protected:

	void BeginPlay() override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void PlaySound();

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void PlayLandingAnimation(const FHitResult& Hit);

    UPROPERTY(VisibleAnywhere, Category=Components)
    UParticleSystemComponent* ParticleSystem;

    UPROPERTY(VisibleAnywhere, Category=Components)
    UBoxComponent* BoxComponent;

    UPROPERTY(VisibleAnywhere, Category=Components)
    USpringArmComponent* CameraSpringArm;

    UPROPERTY(VisibleAnywhere, Category=Components)
    UCameraComponent* Camera;

    const FLinearColor SnowParticleColor;

    float JumpingForce;

    bool bCanDive;
    bool bIsDiving;
    bool bCanPlayJumpSound;

    bool bWaitToPlayWalkingSound;

    float WalkSoundDelay;

    UPROPERTY()
    UWorld* ThisWorld;

    TSharedPtr<DataHolder<USoundCue>> FoxSounds;

    TSharedPtr<DataHolder<UAnimInstance>> FoxAnimations;

    FTimerHandle WalkSoundTimer;

    FTimerHandle ResetWalkSpeed(const float Value, const float Delay);

    FTimerDynamicDelegate WalkingSoundDelegate;
    
};
