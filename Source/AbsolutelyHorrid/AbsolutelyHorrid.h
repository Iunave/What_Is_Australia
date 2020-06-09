// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include <tuple>
#include "CoreMinimal.h"

#define INLINE inline __attribute((always_inline))
#define NO_INLINE __attribute((noinline))

#define LOGS(Message) UE_LOG(HorridLog, Verbose, TEXT(#Message))
#define LOGV(Message, ...) UE_LOG(HorridLog, Verbose, TEXT(#Message), __VA_ARGS__)

#define FIND_OBJECT(Name, Object, Path) static ConstructorHelpers::FObjectFinder<Object>Name##Obj(TEXT(#Path))

DECLARE_LOG_CATEGORY_EXTERN(HorridLog, Log, All)


template<typename DataType>
struct DataHolder
{
    template<typename... Args>
    explicit DataHolder(Args&&... args)
    {
        EmplaceAt_impl(std::forward_as_tuple(Forward<Args>(args)...), Typify<sizeof...(Args)>{});
    }

    ~DataHolder()
    {
        DataArray.Empty();
    }

    TArray<DataType*> DataArray;
private:

    template<size_t N>
    struct Typify { };

    template<typename... Types>
    void EmplaceAt_impl(std::tuple<Types...>&& ArgumentTuple, Typify<0>)
    {
    }
    template<typename... Types, size_t N>
    void EmplaceAt_impl(std::tuple<Types...>&& ArgumentTuple, Typify<N>)
    {
        static const auto CurrentArgument = std::get<sizeof...(Types) - N>(ArgumentTuple);
        DataArray.EmplaceAt(sizeof...(Types) - N, Forward<decltype(CurrentArgument)>(CurrentArgument));

        EmplaceAt_impl(Forward<std::tuple<Types...>>(ArgumentTuple), Typify<N - 1>{});
    }
    template<typename... Types>
    void EmplaceAt_impl(std::tuple<Types...>&& ArgumentTuple, Typify<1>)
    {
        static const auto CurrentArgument = std::get<sizeof...(Types) - 1>(ArgumentTuple);
        DataArray.EmplaceAt(sizeof...(Types) - 1, Forward<decltype(CurrentArgument)>(CurrentArgument));
    }
};
