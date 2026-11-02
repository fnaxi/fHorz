// CopyRight FlareHorz Team. All Rights Reserved.


#include "RendererModule.h"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "Logging/LogCategory.h"
#include "Logging/Logger.h"
#include "ModuleSystem/ModuleManager.h"

DEFINE_PRIVATE_LOG_CATEGORY(lRenderer, All)

void CRendererModule::MountModule()
{
	// ...
}

bool CRendererModule::Initialize(struct SDL_Window* InWindow)
{
	RendererHandle = SDL_CreateRenderer(InWindow, nullptr);
	if (!RendererHandle)
	{
		FH_LOG(Error, lRenderer, "Failed to create SDL_Renderer!")
		return false;
	}
	
	SDL_SetRenderLogicalPresentation(RendererHandle, GetLogicalWindowWidth(), GetLogicalWindowHeight(), SDL_LOGICAL_PRESENTATION_LETTERBOX); // SDL_LOGICAL_PRESENTATION_INTEGER_SCALE
	
	FH_LOG(Info, lRenderer, "Initialized %s", SDL_GetRendererName(RendererHandle))
	return true;
}

void CRendererModule::DetachModule()
{
	SDL_DestroyRenderer(RendererHandle);
}

void CRendererModule::BeginFrame()
{
	ENSURE_STRICT(RendererHandle);
	
	SDL_SetRenderDrawColor(RendererHandle, 0, 0, 0, 255);
	SDL_RenderClear(RendererHandle);
}

void CRendererModule::DrawDebugRect()
{
	const SDL_FRect Rect = { 0.0f, 0.0f, (float)GetLogicalWindowWidth(), (float)GetLogicalWindowHeight() };

	SDL_SetRenderDrawColor(RendererHandle, 150, 0, 0, 255);
	SDL_RenderFillRect(RendererHandle, &Rect);
}

void CRendererModule::EndFrame()
{
	SDL_RenderPresent(RendererHandle);
}

IMPLEMENT_MODULE(CRendererModule, Renderer)

