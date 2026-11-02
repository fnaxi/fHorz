// CopyRight FlareHorz Team. All Rights Reserved.


#include "Containers/String.h"

CString::CString()
{
	Buffer = TArray<CharType>{ NULL_TERMINATOR };
}

CString::CString(const CharType* Text)
{
	const SIZE_T Lenght = STextHelpers::StrLen(Text);
	const ArraySizeType Num = static_cast<ArraySizeType>(Lenght);

	Buffer.SetNum(Num + 1);

	for (ArraySizeType i = 0; i < Num; ++i)
	{
		Buffer.Insert(Text[i], i);
	}

	Buffer.Insert(NULL_TERMINATOR, Num);
}

void CString::Append(const TCHAR Char)
{
	ENSURE_STRICT_M(Buffer.Num() > 0 && Buffer.Last() == NULL_TERMINATOR, "Missing null terminator!");
	
	Buffer.Last() = Char;
	Buffer.Add(NULL_TERMINATOR);
}

void CString::PadRight(const int32 TargetLength)
{
	const int32 SpacesToAdd = TargetLength - Length();
	for (int32 i = 0; i < SpacesToAdd; ++i)
	{
		Append(BLANK_SPACE);
	}
}

bool CString::operator==(const CString& Other) const
{
	if (Length() != Other.Length())
	{
		return false;
	}

	for (ArraySizeType i = 0; i < Length(); ++i)
	{
		if (Buffer[i] != Other.Buffer[i])
		{
			return false;
		}
	}
	
	return true;
}

