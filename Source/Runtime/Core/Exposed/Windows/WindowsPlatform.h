// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#define DEBUG_BREAK() __debugbreak()

struct SWindowsPlatformTypes
{
	/*----------------------------------------------------------------------------
		Signed base types
	----------------------------------------------------------------------------*/

	/** 8-bit signed integer. */
	typedef	signed char			int8;

	/** 16-bit signed integer. */
	typedef signed short int	int16;

	/** 32-bit signed integer. */
	typedef signed int	 		int32;

	/** 64-bit signed integer. */
	typedef signed long long	int64;

	/*----------------------------------------------------------------------------
		Unsigned base types
	----------------------------------------------------------------------------*/

	/** 8-bit unsigned integer. */
	typedef unsigned char 		uint8;

	/** 16-bit unsigned integer. */
	typedef unsigned short int	uint16;

	/** 32-bit unsigned integer. */
	typedef unsigned int		uint32;

	/** 64-bit unsigned integer. */
	typedef unsigned long long	uint64;
	
	/*----------------------------------------------------------------------------
		Character types
	----------------------------------------------------------------------------*/

	/** An ANSI character. 8-bit fixed-width representation of 7-bit characters. */
	typedef char				ANSICHAR;

	/** A wide character. In-memory only. ?-bit fixed-width representation of the platform's natural wide character set. Could be different sizes on different platforms. */
	typedef wchar_t				WIDECHAR;

	/** A switchable character. */
	typedef ANSICHAR			TCHAR;
	
#ifdef PLATFORM_X64
	typedef unsigned __int64	SIZE_T;
	typedef __int64				S_SIZE_T;
#else
	typedef unsigned long		SIZE_T;
	typedef long				S_SIZE_T;
#endif
};

typedef SWindowsPlatformTypes SPlatformTypes;

