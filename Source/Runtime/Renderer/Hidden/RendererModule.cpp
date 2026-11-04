// CopyRight FlareHorz Team. All Rights Reserved.


#include "RendererModule.h"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "RendererDelegates.h"
#include "Logging/LogCategory.h"
#include "Logging/Logger.h"
#include "ModuleSystem/ModuleManager.h"

DEFINE_PRIVATE_LOG_CATEGORY(lRenderer, All)

void CRendererModule::MountModule()
{
	// ...
}

bool CRendererModule::Initialize(SDL_Window* InWindow)
{
	Handle = SDL_CreateRenderer(InWindow, nullptr);
	if (!Handle)
	{
		FH_LOG(Error, lRenderer, "Failed to create SDL_Renderer!")
		return false;
	}

	SDL_SetRenderVSync(Handle, 1);
	
	FH_LOG(Info, lRenderer, "Initialized %s", SDL_GetRendererName(Handle))
	SRendererDelegates::OnInitialized.Broadcast(InWindow, Handle);
	
	return true;
}

void CRendererModule::DetachModule()
{
	SDL_DestroyRenderer(Handle);
}

void CRendererModule::BeginFrame()
{
	SRendererDelegates::OnBeginFrame.Broadcast();
	
	ENSURE_STRICT(Handle);
	
	SDL_SetRenderDrawColor(Handle, 0, 0, 0, 255);
	SDL_RenderClear(Handle);

	SDL_SetRenderLogicalPresentation(Handle, GetLogicalWindowWidth(), GetLogicalWindowHeight(), SDL_LOGICAL_PRESENTATION_LETTERBOX); // SDL_LOGICAL_PRESENTATION_INTEGER_SCALE
}

void CRendererModule::DrawDebugRect()
{
	const SDL_FRect Rect = { 0.0f, 0.0f, (float)GetLogicalWindowWidth(), (float)GetLogicalWindowHeight() };

	SDL_SetRenderDrawColor(Handle, 150, 0, 0, 255);
	SDL_RenderFillRect(Handle, &Rect);
}

void CRendererModule::EndFrame()
{
	SRendererDelegates::OnEndFrame.Broadcast();
	
	SDL_RenderPresent(Handle);
}

IMPLEMENT_MODULE(CRendererModule, Renderer)

