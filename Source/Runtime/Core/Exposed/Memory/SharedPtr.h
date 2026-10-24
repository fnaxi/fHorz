// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include <memory>

#include "Memory.h"

template <typename Type>
using TSharedPtr = std::shared_ptr<Type>;

//@TODO: Shared pointers

template <class T, class... ArgTypes>
TSharedPtr<T> MakeShared(ArgTypes&&... Args) { return std::make_shared<T>(SMemory::Forward<ArgTypes>(Args)...); }
