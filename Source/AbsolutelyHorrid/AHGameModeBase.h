// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/GameModeBase.h"
#include "AHGameModeBase.generated.h"


/** Gamemode class */
UCLASS(MinimalAPI)
class AHGamemodeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

    AHGamemodeBase();

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintImplementableEvent, Blueprintcallable, Category=Level)
    void ReloadMap();

protected:

    virtual void BeginPlay() override;
	
};
