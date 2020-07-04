// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "SnowMan.h"
#include "ScareCrow.generated.h"

/** Enemy scarecrow */
UCLASS(Blueprintable)
class ABSOLUTELYHORRID_API AScareCrow : public ASnowMan
{
	GENERATED_BODY()
public:

    explicit AScareCrow(const FObjectInitializer& ObjectInitializer);
	~AScareCrow() override = default;

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* MeshComponent;

protected:

    virtual void BeginPlay() override;

};
