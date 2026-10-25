// CopyRight FlareHorz Team. All Rights Reserved.


#include "ModuleSystem/ModuleManager.h"

#include "Logging/LogCategory.h"
#include "Logging/Logger.h"
#include "Math/Math.h"

DEFINE_PRIVATE_LOG_CATEGORY(lModuleManager, All)

int32 CModuleInfo::CurrentMountOrder = 1;

CModuleManager& CModuleManager::Get()
{
	static CModuleManager Singleton = CModuleManager();
	return Singleton;
}

IModuleInterface* CModuleManager::GetModule(const SName Name)
{
	const TSharedPtr<CModuleInfo> ModuleInfo = FindModule(Name);
	if (!ModuleInfo || !ModuleInfo->bReady)
	{
		FH_LOG(Error, lModuleManager, "Module %s is not mounted!", *Name)
		return nullptr;
	}
	
	return ModuleInfo->Module.get();
}

IModuleInterface* CModuleManager::MountModule(const SName Name)
{
	const TSharedPtr<CModuleInfo> ModuleInfo = FindModule(Name);
	if (!ModuleInfo)
	{
		FH_LOG(Error, lModuleManager, "Module %s is not registered!", *Name)
		return nullptr;
	}

	if (ModuleInfo->bWasDetachedAtShutdown)
	{
		FH_LOG(Error, lModuleManager, "Module %s was detached at shutdown!", *Name)
		return nullptr;
	}

	IModuleInterface* Module = ModuleInfo->Module.get();
	if (!Module) return nullptr;
	
	if (ModuleInfo->bReady)
	{
		return ModuleInfo->Module.get();
	}
	else
	{
		Module->MountModule();
		ModuleInfo->bReady = true;

		FH_LOG(Info, lModuleManager, "Mounted %s module", *Name)
		
		return Module;
	}
}

IModuleInterface* CModuleManager::MountModuleEnsured(const SName Name)
{
	IModuleInterface* ModuleInterface = MountModule(Name);
	STRICT_ENSURE_M(ModuleInterface, "Failed to mount %s module!", *Name)
	
	return ModuleInterface;
}

void CModuleManager::DetachModule(const SName Name)
{
	TSharedPtr<CModuleInfo> ModuleInfo = FindModule(Name);
	if (!ModuleInfo || !ModuleInfo->bReady)
	{
		FH_LOG(Warning, lModuleManager, "DetachModule() was called but module %s is already detached!", *Name)
		return;
	}

	ModuleInfo->bReady = false;
	ModuleInfo->Module->DetachModule();
	ModuleInfo.reset();

	FH_LOG(Info, lModuleManager, "Detached %s module", *Name)
}

bool CModuleManager::IsModuleMounted(const SName Name) const
{
	const TSharedPtr<CModuleInfo> ModuleInfo = FindModule(Name);
	
	return ModuleInfo && ModuleInfo->bReady;
}

void CModuleManager::RegisterModule(const TCHAR* Name, const CInitializeModuleFunction& InitializeFunction)
{
#define LOG_CATEGORY "ModuleManager"
	if (Modules.Contains(Name)) return;

	if (IModuleInterface* Module = InitializeFunction())
	{
		Modules.Add( Name, MakeShared<CModuleInfo>(Module) );
		
		FH_MISC_LOG(Trace, "Registered module %s", Name)
	}
	else
	{
		FH_MISC_LOG(Error, "The module %s was not constructed properly!", Name)
	}
#undef LOG_CATEGORY
}

void CModuleManager::DetachModulesAtShutdown()
{
	int32 MaxLoadOrder = 0;
	for (const TPair<SName, TSharedPtr<CModuleInfo>>& Module : Modules)
	{
		MaxLoadOrder = SMath::Max(Module.Value->MountOrder, MaxLoadOrder);
	}

	TArray<SName> ModuleNames;
	for (TPair<SName, TSharedPtr<CModuleInfo>>& Module : Modules)
	{
		ModuleNames.Add(Module.Key);
	}
	
	for (int32 Order = MaxLoadOrder; Order >= 0; --Order)
	{
		for (int32 i = 0; i < ModuleNames.Num(); ++i)
		{
			const SName& Name = ModuleNames[i];
			
			if (Modules[Name]->MountOrder == Order)
			{
				if (IsModuleMounted(Name))
				{
					DetachModule(Name);
				}
				if (TSharedPtr<CModuleInfo> ModuleInfo = FindModule(Name))
				{
					ModuleInfo->bWasDetachedAtShutdown = true;
				}
			}
		}
	}
}

TSharedPtr<CModuleInfo> CModuleManager::FindModule(const SName& Name) const
{
	if (const TSharedPtr<CModuleInfo>* FoundModule = Modules.Find(Name))
	{
		return *FoundModule;
	}
	return nullptr;
}

