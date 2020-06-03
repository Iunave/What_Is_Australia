// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "AbsolutelyHorrid.h"
#include "GameFramework/GameModeBase.h"
#include "AbsolutelyHorridGameModeBase.generated.h"

/** Gamemode class */
UCLASS()
class ABSOLUTELYHORRID_API AAbsolutelyHorridGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

    AAbsolutelyHorridGameModeBase();

	virtual void Tick(float DeltaTime) override;

protected:

    virtual void BeginPlay() override;
	
};
