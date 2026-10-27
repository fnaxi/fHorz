// CopyRight FlareHorz Team. All Rights Reserved.


#include "Windows/WindowsPlatformMisc.h"

#include "Windows/WindowsApi.h"

#if !CONFIGURATION_SHIPPING
bool SWindowsPlatformMisc::IsDebuggerPresent()
{
	return !!::IsDebuggerPresent();
}
#endif

