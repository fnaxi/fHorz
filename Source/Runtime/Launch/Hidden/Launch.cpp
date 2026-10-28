// CopyRight FlareHorz Team. All Rights Reserved.


#include "LaunchContext.h"
#include "Misc/CommandLine.h"

#include "SDL3/SDL_main.h"

CLaunchContext GLaunchContext;

int32 GuardedMain()
{
	// Make sure Shutdown() is always called
	struct SLaunchCleanupGuard
	{
		~SLaunchCleanupGuard()
		{
			GLaunchContext.Shutdown();
		}
	} CleanupGuard;
	
	int32 ErrorLevel = GLaunchContext.Configure();
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

int32 SDL_main(int32 ArgC, TCHAR* ArgV[])
{
	CCommandLine::Get().Initialize(ArgC, ArgV);

	int32 ErrorLevel = 0;
	// __try
	{
		//@TODO: crash reporting
		
		// Run the guarded code.
		ErrorLevel = GuardedMain( );
	}
	// __except(  )
	{
		// Avoid warning C6322: Empty __except block
		// (void)0;
	}
	
	return ErrorLevel;
}

