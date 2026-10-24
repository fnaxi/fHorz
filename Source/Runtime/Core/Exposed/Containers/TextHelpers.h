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

class CTextHelpers
{
public:
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

	//@TODO: revisit this
	static TCHAR* MoveTextToHeap(const TCHAR* Text)
	{
		const SIZE_T Lenght = StrLen(Text);

		TCHAR* Name = SMemory::Malloc<TCHAR>(Lenght + 1); 
		for (SIZE_T i = 0; i <= Lenght; ++i)
		{
			Name[i] = Text[i];
		}
	
		return Name;
	}
};
