// CopyRight FlareHorz Team. All Rights Reserved.


#include "ModuleSystem/ModuleManager.h"

#include "Logging/LogCategory.h"
#include "Logging/Logger.h"
#include "Math/Math.h"

#define LOG_CATEGORY "ModuleManager" // Used in RegisterModule() method

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
		FH_LOG(Error, lModuleManager, "Tried to get interface for unmounted module %s!", *Name)
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
		FH_LOG(Error, lModuleManager, "Can't mount module %s because it was detached at shutdown!", *Name)
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
		ModuleInfo->MountOrder = CModuleInfo::CurrentMountOrder++;

		FH_LOG(Info, lModuleManager, "Mounted %s module", *Name)
		return Module;
	}
}

IModuleInterface* CModuleManager::MountModuleEnsured(const SName Name)
{
	IModuleInterface* ModuleInterface = MountModule(Name);
	ENSURE_STRICT_M(ModuleInterface, "Failed to mount %s module!", *Name);
	
	return ModuleInterface;
}

void CModuleManager::DetachModule(const SName Name)
{
	TSharedPtr<CModuleInfo> ModuleInfo = FindModule(Name);
	if (!ModuleInfo || !ModuleInfo->bReady)
	{
		FH_LOG(Warn, lModuleManager, "DetachModule() was called but module %s is already detached!", *Name)
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
	if (Modules.Contains(Name)) return;

	if (IModuleInterface* Module = InitializeFunction())
	{
		Modules.Add( Name, MakeShared<CModuleInfo>(Module) );
		
		FH_MISC_LOG(Trace, "Registered module %s", Name)
	}
	else
	{
		FH_MISC_LOG(Error, "The module %s was not initialized properly!", Name)
	}
}

void CModuleManager::DetachModulesAtShutdown()
{
	struct SModuleDetachEntry
	{
		SModuleDetachEntry(const SName InName, const int32 InMountOrder)
			: Name(InName), MountOrder(InMountOrder) { }

		SName Name;
		int32 MountOrder;
	};

	TArray<SModuleDetachEntry> ModuleEntries;
	for (const TPair<SName, TSharedPtr<CModuleInfo>>& Module : Modules)
	{
		if( !!Module.Value && Module.Value->bReady )
		{
			ModuleEntries.Add( SModuleDetachEntry(Module.Key, Module.Value->MountOrder) );
		}
	}

	ModuleEntries.Sort([](const SModuleDetachEntry& A, const SModuleDetachEntry& B) arrow( A.MountOrder > B.MountOrder ));

	for (const SModuleDetachEntry& Entry : ModuleEntries)
	{
		if (IsModuleMounted(Entry.Name))
		{
			DetachModule(Entry.Name);
		}
        
		if (const TSharedPtr<CModuleInfo> ModuleInfo = FindModule(Entry.Name))
		{
			ModuleInfo->bWasDetachedAtShutdown = true;
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

#undef LOG_CATEGORY

