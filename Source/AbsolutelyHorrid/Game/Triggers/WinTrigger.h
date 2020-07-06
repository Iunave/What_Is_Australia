// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Actor.h"
#include "WinTrigger.generated.h"

class UBoxComponent;

UCLASS()
class ABSOLUTELYHORRID_API AWinTrigger : public AActor
{
	GENERATED_BODY()
public:	

	AWinTrigger();

    virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;
	
};
