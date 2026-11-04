// CopyRight FlareHorz Team. All Rights Reserved.


#include "Engine.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include "EngineDelegates.h"
#include "RendererModule.h"
#include "Logging/Logger.h"
#include "ModuleSystem/ModuleManager.h"

DEFINE_LOG_CATEGORY(lEngine)

ENGINE_API CEngine* GEngine = nullptr;

int32 CEngine::Initialize()
{
	if( !SDL_Init(SDL_INIT_VIDEO) )
	{
		FH_LOG(Error, lEngine, "Failed to initialize SDL_INIT_VIDEO!")
		return false;
	}

	constexpr SDL_WindowFlags WindowFlags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY; // SDL_WINDOW_FULLSCREEN
	
	Window = SDL_CreateWindow("fHorz Runtime", WindowWidth, WindowHeight, WindowFlags);
	if (!Window)
	{
		FH_LOG(Error, lEngine, "Failed to create the window!")
		return false;
	}

	// SDL_SetWindowPosition(Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	
	FH_LOG(Info, lEngine, "Created the window with %dx%d resolution", WindowWidth, WindowHeight)
	SEngineDelegates::OnWindowCreated.Broadcast();
	
	Renderer = CModuleManager::Get().GetModule<CRendererModule>("Renderer");
	ENSURE_STRICT(Renderer);
	
	Renderer->Initialize(Window);
	SDL_ShowWindow(Window);
	
	//@TODO: input manager
	{
		KeysState = SDL_GetKeyboardState(nullptr);
	}
	
	FH_LOG(Info, lEngine, "Engine initialized")
	return 0;
}

void CEngine::Tick()
{
	const uint64 TickTime = SDL_GetTicksNS();
	if (LastTickTime == 0)
	{
		LastTickTime = TickTime;
		return;
	}
	
	const float DeltaTime = static_cast<float>(TickTime - LastTickTime) / (1000.0f * 1000.0f * 1000.0f);
	LastTickTime = TickTime;

	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		SEngineDelegates::OnProcessEvent.Broadcast(&Event);
		switch (Event.type)
		{
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			RequestExit("SDL_EVENT_WINDOW_CLOSE_REQUESTED");
			break;
		case SDL_EVENT_WINDOW_RESIZED:
			WindowWidth = Event.window.data1;
			WindowHeight = Event.window.data2;
			FH_LOG(Debug, lEngine, "Resized window to %dx%d resolution", WindowWidth, WindowHeight)
			break;
		}
	}
	
	PullInput();
	Update(DeltaTime);
	Render();
}

void CEngine::PullInput()
{
	if (KeysState[SDL_SCANCODE_ESCAPE])
	{
		RequestExit("SDL_SCANCODE_ESCAPE pressed");
	}
}

void CEngine::Update(const float DeltaTime)
{
	// ...
}

void CEngine::Render()
{
	Renderer->BeginFrame();

	Renderer->DrawDebugRect();
	
	Renderer->EndFrame();
}

void CEngine::Shutdown()
{
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void CEngine::RequestExit(const TCHAR* Reason)
{
	FH_LOG(Info, lEngine, "Engine exit requested (reason: %s%s)", Reason, bRequestingExit ? "; note: exit was already requested" : "")
	bRequestingExit = true;
}

