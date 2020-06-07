// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "../AbsolutelyHorrid.h"
#include "GameFramework/Character.h"
#include "FoxCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UAnimationAsset;
class UAnimBlueprint;
class USoundCue;
struct FTimerHandle;
class FTimerDynamicDelegate;


UCLASS(Blueprintable)
class ABSOLUTELYHORRID_API AFoxCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AFoxCharacter();
	~AFoxCharacter() override;

    void Tick(float DeltaTime) override;


    inline void MoveForward(float Value);

    inline void MoveRight(float Value);

    inline void Jump() override;

    inline void Dive();

    void PlayLandingAnimation(const FHitResult& Hit);

protected:

	void BeginPlay() override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void PlaySound();

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category=Components)
	USkeletalMeshComponent* SkeletalMesh;

    UPROPERTY(VisibleAnywhere, Category=Components)
    USpringArmComponent* CameraSpringArm;

    UPROPERTY(VisibleAnywhere, Category=Components)
    UCameraComponent* Camera;

    float TurnSensitivity;
    float JumpingForce;

    bool bCanDive;
    bool bIsDiving;

    bool bWaitToPlayWalkingSound;

    float WalkSoundDelay;


    TSharedPtr<DataHolder<USoundCue>> FoxSounds;

    TSharedPtr<DataHolder<UAnimBlueprint>> FoxAnimations;

    FTimerHandle WalkingSoundTimerHandle;

    FTimerDynamicDelegate WalkingSoundDelegate;
    
};