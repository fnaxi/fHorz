// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "DebugTab.h"

class CToolSettingsDebugTab : public IDebugTab
{
public:	
	virtual void OnDrawGUI() override;
	virtual const TCHAR* GetTabName() const override arrow ("Tool Settings")

private:
	float Transparency = 1.0f;
};
