// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "ModuleSystem/ModuleInterface.h"

class CDebugGUIModule : public IModuleInterface
{
public:
	virtual void MountModule() override;
	virtual void DetachModule() override;
};
