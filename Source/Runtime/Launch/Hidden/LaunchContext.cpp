// CopyRight FlareHorz Team. All Rights Reserved.


#include "LaunchContext.h"

#include "Engine.h"
#include "Logging/Logger.h"
#include "ModuleSystem/ModuleManager.h"

DEFINE_PRIVATE_LOG_CATEGORY(lLaunch, All)

int32 CLaunchContext::PreInitialize()
{
	// CModuleManager::Get().MountModuleEnsured("Core");
	return 0;
}

int32 CLaunchContext::Initialize()
{
	CModuleManager::Get().MountModuleEnsured("Engine");
	ENSURE_STRICT(GEngine);
	
	CModuleManager::Get().MountModuleEnsured("Renderer");

#if WITH_TOOLKIT
	CModuleManager::Get().MountModuleEnsured("DebugGUI");
#endif
	
	const int32 InitResult = GEngine->Initialize();
	if (InitResult != 0)
	{
		FH_LOG(Error, lLaunch, "Failed to initialize engine!")
			
		return InitResult;
	}
	
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

