// CopyRight FlareHorz Team. All Rights Reserved.


#include "LaunchContext.h"

#include "Engine.h"
#include "ModuleSystem/ModuleManager.h"

int32 CLaunchContext::PreInitialize(int32 ArgC, TCHAR* ArgV[])
{
	//@TODO: Command line
	return PreInitialize("");
}

int32 CLaunchContext::PreInitialize(const TCHAR* CmdLine)
{
	CModuleManager::Get().MountModule("Core");
	
	return 0;
}

int32 CLaunchContext::Initialize()
{
	CModuleManager::Get().MountModule("Engine");
	
	return 0;
}

void CLaunchContext::RunEngineLoop()
{
	while( GEngine && !GEngine->IsExitRequested() )
	{
		GEngine->Tick();
	}
}

void CLaunchContext::Shutdown()
{
	CModuleManager::Get().DetachModulesAtShutdown();
}

