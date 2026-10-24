// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Platform/Platform.h"

#define REGISTER_NAME(Index, Name) Name = Index,
enum class EName : uint8
{
	#include "FlareNames.inl"

	MaxHardcodedNameIndex
};
#undef REGISTER_NAME

