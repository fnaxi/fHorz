// CopyRight FlareHorz Team. All Rights Reserved.


#include "Containers/TextHelpers.h"

SIZE_T CTextHelpers::StrLen(const TCHAR* Text)
{
	return std::strlen(Text);
}

int32 CTextHelpers::Compare(const TCHAR* A, const TCHAR* B, const ETextCase CaseSensitivity)
{
	if (CaseSensitivity == ETextCase::CaseSensitive)
	{
#if TCHAR_IS_ANSICHAR
		return std::strcmp(A, B);
#else
		return std::wcscmp(A, B);
#endif
	}
	else
	{
#if PLATFORM_WINDOWS
	#if TCHAR_IS_ANSICHAR
		return _stricmp(A, B);
	#else
		return _wcsicmp(A, B);
	#endif
#else
		return false;
#endif
	}
}
