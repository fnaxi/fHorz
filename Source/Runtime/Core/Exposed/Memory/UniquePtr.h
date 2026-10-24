// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include <memory>

#include "Memory.h"

//@TODO: Unique pointers

template <typename Type>
using TUniquePtr = std::unique_ptr<Type>;

template <class T, class... ArgTypes>
TSharedPtr<T> MakeUnique(ArgTypes&&... Args) { return std::make_unique<T>(SMemory::Forward<ArgTypes>(Args)...); }
