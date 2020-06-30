// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Actor.h"
#include "Boulder.generated.h"

class UStaticMeshComponent;
class USoundCue;
class USphereComponent;

UCLASS()
class ABSOLUTELYHORRID_API ABoulder : public AActor
{
	GENERATED_BODY()

	friend class AFoxCharacter;
	
public:	

	ABoulder();

    virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category=Components)
	USceneComponent* Root;

    UPROPERTY(EditAnywhere, Category=Components)
    USphereComponent* OverlapSphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
    UStaticMeshComponent* Mesh;

    UPROPERTY()
    TArray<USoundCue*> RollingSounds;
	
};
