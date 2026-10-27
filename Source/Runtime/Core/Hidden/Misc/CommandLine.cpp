// CopyRight FlareHorz Team. All Rights Reserved.


#include "Misc/CommandLine.h"

#include "Containers/String.h"
#include "Logging/LogCategory.h"
#include "Logging/Logger.h"
#include "Memory/Memory.h"

DEFINE_PRIVATE_LOG_CATEGORY(lCommandLine, All)

CCommandLine::~CCommandLine()
{
	SMemory::Free(CmdLine);
}

CCommandLine& CCommandLine::Get()
{
	static CCommandLine Instance;
	return Instance;
}

void CCommandLine::Initialize(int32 ArgC, TCHAR* ArgV[])
{
	if (ArgC <= 0 || !ArgV) return;

	SIZE_T TotalLength = 0;
	for (int32 i = 0; i < ArgC; ++i)
	{
		if (!ArgV[i]) continue;
		
		TotalLength += STextHelpers::StrLen(ArgV[i]) + 1;
	}

	if (TotalLength <= 1) return;

	CmdLine = SMemory::Malloc<TCHAR>(TotalLength);
	
	TCHAR* CurrentPosition = CmdLine;
	SIZE_T RemainingSize = TotalLength;

	for (int32 i = 1; i < ArgC; ++i)
	{
		if (!ArgV[i]) continue;

		const int32 Format = STextHelpers::Format(CurrentPosition, RemainingSize, "%s", ArgV[i]);
		if (Format > 0)
		{
			CurrentPosition += Format;
			RemainingSize -= Format;
		}

		if (i < ArgC - 1 && RemainingSize > 1)
		{
			*CurrentPosition = CString::BLANK_SPACE;
			CurrentPosition++;
			RemainingSize--;
			*CurrentPosition = CString::NULL_TERMINATOR;
		}
	}

	FH_LOG(Info, lCommandLine, "Passed: %s", CmdLine)
}


bool CCommandLine::ParseParam(const TCHAR* Param)
{
	return true;
}

bool CCommandLine::ParseValue(const TCHAR* Match, TCHAR* OutValue)
{
	//@TODO: parse value
	return false;
}

