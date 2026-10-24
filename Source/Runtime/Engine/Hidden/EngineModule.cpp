// CopyRight FlareHorz Team. All Rights Reserved.


#include "EngineModule.h"

#include "Engine.h"
#include "ModuleSystem/ModuleManager.h"

void CEngineModule::MountModule()
{
	GEngine = NewObject<CEngine>();
	if (GEngine)
	{
		GEngine->Initialize();
	}
}

void CEngineModule::DetachModule()
{
	if (GEngine)
	{
		GEngine->Shutdown();
	}
	
	DestroyObject(GEngine);
}

IMPLEMENT_MODULE(CEngineModule, Engine)

