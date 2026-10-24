// CopyRight FlareHorz Team. All Rights Reserved.


#include "LaunchContext.h"

CLaunchContext GLaunchContext;

int32 main(int32 ArgC, TCHAR* ArgV[])
{
	// Make sure Shutdown() is always called
	struct SLaunchCleanupGuard
	{
		~SLaunchCleanupGuard()
		{
			GLaunchContext.Shutdown();
		}
	} CleanupGuard;

	int32 ErrorLevel = GLaunchContext.PreInitialize(ArgC, ArgV);
	if (ErrorLevel != 0)
	{
		return ErrorLevel;
	}

	ErrorLevel = GLaunchContext.Initialize();
	if (ErrorLevel != 0)
	{
		return ErrorLevel;
	}

	GLaunchContext.RunEngineLoop();
	
	return ErrorLevel;
}

