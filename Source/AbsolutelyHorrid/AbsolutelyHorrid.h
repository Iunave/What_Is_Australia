// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#define INLINE inline __attribute((always_inline))
#define NO_INLINE __attribute((noinline))

DECLARE_LOG_CATEGORY_EXTERN(HorridLog, Log, All);

#define LOGS(Message) UE_LOG(HorridLog, Verbose, TEXT(#Message))
#define LOGV(Message, ...) UE_LOG(HorridLog, Verbose, TEXT(#Message), __VA_ARGS__)

#define FIND_OBJECT(Name, Object, Path) static ConstructorHelpers::FObjectFinder<Object>Name##Obj(TEXT(#Path))


