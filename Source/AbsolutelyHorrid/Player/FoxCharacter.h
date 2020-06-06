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
class USoundCue;


template<typename... DataType>
struct DataHolder
{
    explicit inline DataHolder(DataType*... Type);

    ~DataHolder() = default;

    TArray<DataType*...> DataArray;
};


UCLASS(Blueprintable)
class ABSOLUTELYHORRID_API AFoxCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AFoxCharacter();
	~AFoxCharacter();

    void Tick(float DeltaTime) override;


    inline void MoveForward(float Value);

    inline void MoveRight(float Value);

    inline void Jump() override;

    inline void Dive();

    void PlayLandingAnimation(const FHitResult& Hit);

protected:

	void BeginPlay() override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

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


    TSharedPtr<DataHolder<USoundCue>> FoxSounds;

    TSharedPtr<DataHolder<UAnimationAsset>> FoxAnimations;

};
