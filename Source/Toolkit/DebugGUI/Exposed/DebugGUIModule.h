// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "ModuleSystem/ModuleInterface.h"

class CDebugGUIModule : public IModuleInterface
{
public:
	virtual void MountModule() override;
	virtual void DetachModule() override;

private:
	void OnWindowCreated();
	void OnProcessEvent(union SDL_Event* Event);
	
	void OnRendererInitialized(struct SDL_Window* Window, struct SDL_Renderer* InRenderer);

	void OnBeginFrame();
	void OnEndFrame();

	void DrawGUI();

	template<typename TabType>
	void AddDebugTab()
	{
		Tabs.Add(NewObject<TabType>());
	}

	TArray<class IDebugTab*> Tabs;
	SDL_Renderer* Renderer = nullptr;

	bool bShowDebugGUI = false;
};
