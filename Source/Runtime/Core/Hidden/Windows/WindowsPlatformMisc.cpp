// CopyRight FlareHorz Team. All Rights Reserved.


#include "Windows/WindowsPlatformMisc.h"

#include "Windows/WindowsApi.h"

bool SWindowsPlatformMisc::IsDebuggerPresent()
{
	return !!::IsDebuggerPresent();
}

