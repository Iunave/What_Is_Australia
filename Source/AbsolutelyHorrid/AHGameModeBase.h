// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "AbsolutelyHorrid.h"
#include "GameFramework/GameModeBase.h"
#include "AHGameModeBase.generated.h"

/** Gamemode class */
UCLASS()
class ABSOLUTELYHORRID_API AHGamemodeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

    AHGamemodeBase();

    virtual void Tick(float DeltaTime) override;

protected:

    virtual void BeginPlay() override;
	
};
