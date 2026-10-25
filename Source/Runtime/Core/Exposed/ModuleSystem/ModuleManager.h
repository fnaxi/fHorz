// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "ModuleInterface.h"
#include "Memory/SharedPtr.h"
#include "Memory/UniquePtr.h"
#include "Containers/Map.h"
#include "Containers/Name.h"

using CInitializeModuleFunction = IModuleInterface* (*)();

class CModuleInfo
{
public:
	CModuleInfo(IModuleInterface* InInterface)
		: Module(InInterface)
		, MountOrder(CurrentMountOrder++)
	{ }

	~CModuleInfo() = default;
	
	/** The module object for this module. We actually *own* this module, so it's lifetime is controlled by the scope of this shared pointer. */
	TUniquePtr<IModuleInterface> Module;
	
	/** True if this module is mounted and ready to be used */
	bool			bReady = false;

	/** True if this module was detached at shutdown time, and we never want it to be mounted again. */
	bool			bWasDetachedAtShutdown = false;

	/** Arbitrary number that encodes the mount order of this module, so we can shut them down in reverse order. */
	int32			MountOrder;
	
	/** Static that tracks the current mount number. Incremented whenever a new module is mounted. */
	static int32	CurrentMountOrder;
};

class CModuleManager final
{
	CModuleManager() = default;
	
public:
	static CModuleManager& Get();

	IModuleInterface* GetModule(SName Name);
	
	IModuleInterface* MountModule(SName Name);
	IModuleInterface* MountModuleEnsured(SName Name);

	void DetachModule(SName Name);
	void DetachModulesAtShutdown();

	void RegisterModule(const TCHAR* Name, const CInitializeModuleFunction& InitializeFunction);

	[[nodiscard]] bool IsModuleMounted(SName Name) const;

	[[nodiscard]] int32 GetModuleCount() const arrow( Modules.Num() )

private:
	TMap<SName, TSharedPtr<CModuleInfo>> Modules;

	[[nodiscard]] TSharedPtr<CModuleInfo> FindModule(const SName& Name) const;
};

class CModuleRegistration final
{
public:
	CModuleRegistration(const TCHAR* Name, const CInitializeModuleFunction& Function)
	{
		CModuleManager::Get().RegisterModule(Name, Function);
	}
};

namespace Flare::Core::Private
{
	constexpr bool ModuleNameEquals(const TCHAR* Lhs, const TCHAR* Rhs)
	{
		for (;;)
		{
			if (*Lhs != *Rhs) return false;
			if (*Lhs == '\0') return true;
			++Lhs;
			++Rhs;
		}
	}
}

#define IMPLEMENT_MODULE( ModuleImplClass, ModuleName ) \
	static IModuleInterface* Z_Initialize##ModuleName##Module() \
	{ \
		return new ModuleImplClass(); \
	} \
	static CModuleRegistration Z_##ModuleName##ModuleRegistration(#ModuleName, Z_Initialize##ModuleName##Module); \
	\
	extern "C" void Z_Implement##ModuleName##Module() \
	{ \
		static_assert(Flare::Core::Private::ModuleNameEquals(#ModuleName, FH_STRINGIZE(FH_MODULE_NAME)), \
			"Module name mismatch (" #ModuleName " != " FH_STRINGIZE(FH_MODULE_NAME) "). " \
			"Please ensure module name passed to IMPLEMENT_MODULE is " FH_STRINGIZE(FH_MODULE_NAME) "!"); \
	}
