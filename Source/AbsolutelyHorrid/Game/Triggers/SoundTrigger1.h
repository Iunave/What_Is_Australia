// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Actor.h"
#include "SoundTrigger1.generated.h"

class UBoxComponent;

UCLASS()
class ABSOLUTELYHORRID_API ASoundTrigger1 : public AActor
{
	GENERATED_BODY()
public:	

	ASoundTrigger1();

    virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    UBoxComponent* Box;
	
};
