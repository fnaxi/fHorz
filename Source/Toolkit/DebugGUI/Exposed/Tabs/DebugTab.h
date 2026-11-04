// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Platform/Platform.h"
#include "Misc/CoreDefines.h"

class IDebugTab
{
public:
	IDebugTab() = default;
	virtual ~IDebugTab() = default;
	
	virtual void OnDrawGUI() = 0;

	virtual const TCHAR* GetTabName() const arrow ("Untitled")

protected:
	void BeginSection(const TCHAR* Name);
	void EndSection();

	void DrawEmptyLine();
};
