// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Misc/PreprocessorHelpers.h"

#include PLATFORM_HEADER(Platform.h)

#if !PLATFORM_WINDOWS || !defined(_MSC_VER)
	#error Only Windows is supported right now!
#endif

#define TCHAR_IS_ANSICHAR 1

/*----------------------------------------------------------------------------
	Transfer the platform types to global types
----------------------------------------------------------------------------*/

// Signed base types
typedef	SPlatformTypes::int8		int8;
typedef SPlatformTypes::int16		int16;
typedef SPlatformTypes::int32		int32;
typedef SPlatformTypes::int64		int64;

// Unsigned base types
typedef SPlatformTypes::uint8		uint8;
typedef SPlatformTypes::uint16		uint16;
typedef SPlatformTypes::uint32		uint32;
typedef SPlatformTypes::uint64		uint64;

// Character types
typedef SPlatformTypes::ANSICHAR	ANSICHAR;
typedef SPlatformTypes::WIDECHAR	WIDECHAR;
typedef SPlatformTypes::TCHAR		TCHAR;

// Misc types
typedef SPlatformTypes::SIZE_T		SIZE_T;
typedef SPlatformTypes::S_SIZE_T	S_SIZE_T;
