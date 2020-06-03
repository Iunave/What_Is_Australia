// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "../AbsolutelyHorrid.h"
#include "GameFramework/Pawn.h"
#include "HorridPawn.generated.h"


UCLASS(Blueprintable)
class ABSOLUTELYHORRID_API AHorridPawn : public APawn
{
	GENERATED_BODY()

public:

	AHorridPawn();

    virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category="Components")
	USceneComponent* SceneComponent;

};
