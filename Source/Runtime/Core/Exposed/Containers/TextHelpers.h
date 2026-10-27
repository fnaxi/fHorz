// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Memory/Memory.h"

#if TCHAR_IS_ANSICHAR
	#include <cstdio>
#else
	#include <cwchar>
#endif

enum class ETextCase : uint8
{
	CaseSensitive,
	IgnoreCase
};

struct STextHelpers
{
	static SIZE_T StrLen(const TCHAR* Text);

	template <typename... ArgTypes>
	static int32 Format(TCHAR* Buffer, SIZE_T Size, const TCHAR* Format, ArgTypes&&... Args)
	{
#if TCHAR_IS_ANSICHAR
		return std::snprintf(Buffer, Size, Format, SMemory::Forward<ArgTypes>(Args)...);
#else
		return std::swprintf(Buffer, Size, Format, SMemory::Forward<ArgTypes>(Args)...);
#endif
	}

	/**
	 * Compares two strings.
	 * @returns:
	 * - 0 if strings are identical.
	 * - a negative if A comes before string B alphabetically.
	 * - positive if A comes after string B alphabetically.
	 */
	static int32 Compare(const TCHAR* A, const TCHAR* B, ETextCase CaseSensitivity);
};
