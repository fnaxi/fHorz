// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Array.h"
#include "TextHelpers.h"

/**
 * A dynamically sizeable string.
 *
 * The internal buffer is always null-terminated, allowing safe access as a C-style string via CStr() or operator*().
 */
class CString
{
	enum : uint8
	{
		NULL_TERMINATOR = '\0',
		BLANK_SPACE = ' '
	};
	
	using CharType = TCHAR;
	
public:
	
	//@TODO: String is only partly implemented

	CString();
	CString(const CharType* Text);

	/** Creates a formatted string using printf-style formatting. */
	template <typename... ArgTypes>
	[[nodiscard]] static CString Printf(const CharType* Format, ArgTypes&&... Args)
	{
		const int32 Size = CTextHelpers::Format(nullptr, 0, Format, SMemory::Forward<ArgTypes>(Args)...);
		if (Size <= 0)
		{
			return CString();
		}
		
		CString Result;
		
		Result.Buffer.SetNum(Size + 1);
		CTextHelpers::Format(Result.Buffer.GetData(), Size + 1, Format, SMemory::Forward<ArgTypes>(Args)...);
		Result.Buffer[Size] = NULL_TERMINATOR;
		
		return Result;
	}

	void Append(TCHAR Char);
	void PadRight(int32 TargetLength);

	/** Get the length of the string, excluding terminating character. */
	[[nodiscard]] ArraySizeType Length() const
	{
		return Buffer.Num() > 0 ? Buffer.Num() - 1 : 0;
	}

	[[nodiscard]] FORCEINLINE CharType* CStr() arrow( Buffer.GetData() )
	[[nodiscard]] FORCEINLINE const CharType* CStr() const arrow( Buffer.GetData() )
	
private:
	
	/** Array holding the character data. */
	TArray<CharType> Buffer;

public:

	/*----------------------------------------------------------------------------
		Operators
	----------------------------------------------------------------------------*/

	const CharType* operator*() const arrow( CStr() )

	CharType&		operator[](ArraySizeType Index)			arrow( Buffer[Index] )
	const CharType& operator[](ArraySizeType Index) const	arrow( Buffer[Index] )

	bool operator==(const CString& Other) const;
};
