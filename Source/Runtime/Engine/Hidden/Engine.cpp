// CopyRight FlareHorz Team. All Rights Reserved.


#include "Engine.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

#include "Logging/Logger.h"

DEFINE_LOG_CATEGORY(lEngine)

ENGINE_API CEngine* GEngine = nullptr;

int32 CEngine::Initialize()
{
	if( !SDL_Init(SDL_INIT_VIDEO) )
	{
		FH_LOG(Error, lEngine, "Failed to initialize SDL_INIT_VIDEO!")
		return false;
	}

	Window = SDL_CreateWindow("fHorz Runtime", 1280, 720, 0);
	if (!Window)
	{
		FH_LOG(Error, lEngine, "Failed to create a window!")
		return false;
	}

	Renderer = SDL_CreateRenderer(Window, nullptr);
	if (!Renderer)
	{
		FH_LOG(Error, lEngine, "Failed to create renderer!")
		return false;
	}
	
	FH_LOG(Info, lEngine, "Engine initialized")
	return 0;
}

void CEngine::PullInput()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_EVENT_QUIT:
			RequestExit("SDL_EVENT_QUIT");
			break;
		}
	}
}

void CEngine::Update()
{
	//@TODO: DeltaTime
	// ...
}

void CEngine::Render()
{
	STRICT_ENSURE(Renderer);
	
	SDL_SetRenderDrawColor(Renderer, 50, 50, 50, 255);
	SDL_RenderClear(Renderer);

	SDL_RenderPresent(Renderer);
}

void CEngine::Tick()
{
	PullInput();
	Update();
	Render();
}

void CEngine::Shutdown()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void CEngine::RequestExit(const TCHAR* Reason)
{
	FH_LOG(Info, lEngine, "Engine exit requested (reason: %s%s)", Reason, bRequestingExit ? "; note: exit was already requested" : "")
	bRequestingExit = true;
}

