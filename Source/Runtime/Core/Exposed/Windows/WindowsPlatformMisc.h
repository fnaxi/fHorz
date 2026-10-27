// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

struct SWindowsPlatformMisc
{
#if !CONFIGURATION_SHIPPING
	static bool IsDebuggerPresent();
#endif
};

typedef SWindowsPlatformMisc SPlatformMisc;
