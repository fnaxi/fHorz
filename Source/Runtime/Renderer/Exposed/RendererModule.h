// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "ModuleSystem/ModuleInterface.h"
#include "Platform/Platform.h"
#include "Misc/CoreDefines.h"

class CRendererModule : public IModuleInterface
{
public:
	virtual void MountModule() override;
	bool Initialize(struct SDL_Window* InWindow);
	
	virtual void DetachModule() override;
	
	void BeginFrame();
	void DrawDebugRect();
	void EndFrame();

	/** Alternatives: 1280x640, 960x480, 640x320. */
	constexpr int32 GetLogicalWindowWidth() arrow (1920)
	constexpr int32 GetLogicalWindowHeight() arrow (960)
	
private:
	struct SDL_Renderer* RendererHandle;
};
