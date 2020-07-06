// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Actor.h"
#include "SoundTrigger2.generated.h"

class UBoxComponent;

UCLASS()
class ABSOLUTELYHORRID_API ASoundTrigger2 : public AActor
{
	GENERATED_BODY()
public:	

	ASoundTrigger2();

    virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    UBoxComponent* Box;

};
