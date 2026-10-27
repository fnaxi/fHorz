// CopyRight FlareHorz Team. All Rights Reserved.


#include "LaunchContext.h"

#include "Engine.h"
#include "Logging/Logger.h"
#include "ModuleSystem/ModuleManager.h"

int32 CLaunchContext::Configure()
{
	//CModuleManager::Get().MountModule("Core");

	return 0;
}

int32 CLaunchContext::Initialize()
{
	CModuleManager::Get().MountModuleEnsured("Engine");
	STRICT_ENSURE_M(GEngine, "Failed to create global engine instance!");

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

