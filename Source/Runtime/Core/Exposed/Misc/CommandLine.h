// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Platform/Platform.h"

class CCommandLine
{
	CCommandLine() = default;
	~CCommandLine();

public:
	CCommandLine(const CCommandLine&) = delete;
	CCommandLine& operator=(const CCommandLine&) = delete;
	
	static CCommandLine& Get();
	
	void Initialize(int32 ArgC, TCHAR* ArgV[]);
	
	bool ParseParam(const TCHAR* Param);
	bool ParseValue(const TCHAR* Match, TCHAR* OutValue);

private:
	TCHAR* CmdLine = nullptr;
};

