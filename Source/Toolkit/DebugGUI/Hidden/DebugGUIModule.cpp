// CopyRight FlareHorz Team. All Rights Reserved.


#include "DebugGUIModule.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "ModuleSystem/ModuleManager.h"
#include "Logging/LogCategory.h"
#include "EngineDelegates.h"
#include "RendererDelegates.h"
#include "Delegates/Delegate.h"
#include "Logging/Logger.h"
#include "Tabs/ToolSettingsDebugTab.h"

DEFINE_PRIVATE_LOG_CATEGORY(lDebugGUI, All)

void CDebugGUIModule::MountModule()
{
	SEngineDelegates::OnWindowCreated.Subscribe(this, &CDebugGUIModule::OnWindowCreated);
	SEngineDelegates::OnProcessEvent.Subscribe(this, &CDebugGUIModule::OnProcessEvent);

	SRendererDelegates::OnInitialized.Subscribe(this, &CDebugGUIModule::OnRendererInitialized);
	SRendererDelegates::OnBeginFrame.Subscribe(this, &CDebugGUIModule::OnBeginFrame);
	SRendererDelegates::OnEndFrame.Subscribe(this, &CDebugGUIModule::OnEndFrame);
	
	AddDebugTab<CToolSettingsDebugTab>();
}

void CDebugGUIModule::DetachModule()
{
	SEngineDelegates::OnWindowCreated.UnsubscribeAll(this);
	SEngineDelegates::OnProcessEvent.UnsubscribeAll(this);
	
	SRendererDelegates::OnInitialized.UnsubscribeAll(this);
	SRendererDelegates::OnBeginFrame.UnsubscribeAll(this);
	SRendererDelegates::OnEndFrame.UnsubscribeAll(this);
	
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();

	ImGui::DestroyContext();
	
	for (IDebugTab* Tab : Tabs)
	{
		DestroyObject(Tab);
	}
}

void CDebugGUIModule::OnWindowCreated()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	ImGuiIO& IO = ImGui::GetIO(); (void)IO;
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// Enable Gamepad Controls
	IO.IniFilename = nullptr;
	
	// Setup Dear ImGui style
	ImGuiStyle& Style = ImGui::GetStyle();

	ImGui::StyleColorsDark(); // ImGui::StyleColorsLight();
	
	Style.FrameRounding = 3.0f;
	Style.GrabRounding = 3.0f;

	FH_LOG(Info, lDebugGUI, "Created ImGui context")
}

void CDebugGUIModule::OnProcessEvent(SDL_Event* Event)
{
	ImGui_ImplSDL3_ProcessEvent(Event);
}

void CDebugGUIModule::OnRendererInitialized(SDL_Window* Window, SDL_Renderer* InRenderer)
{
	Renderer = InRenderer;
	
	// Setup platform/renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(Window, Renderer);
	ImGui_ImplSDLRenderer3_Init(Renderer);

	FH_LOG(Info, lDebugGUI, "Initialized ImGui for SDL renderer")
}

void CDebugGUIModule::OnBeginFrame()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void CDebugGUIModule::OnEndFrame()
{
	// ImGui::ShowDemoWindow();

	ImGui::SetNextWindowCollapsed(true, ImGuiCond_FirstUseEver);
	if (ImGui::GetIO().KeyShift && ImGui::IsKeyPressed(ImGuiKey_X))
	{
		bShowDebugGUI = !bShowDebugGUI;
		ImGui::SetNextWindowCollapsed(!bShowDebugGUI, ImGuiCond_Always);
	}

	ImGui::SetNextWindowPos(ImVec2(25, 21));
	ImGui::SetNextWindowSize(ImVec2(870, 390));
	
	float MsPerFrame = 1000.0f / ImGui::GetIO().Framerate;
	float Framerate = ImGui::GetIO().Framerate;

	TCHAR TitleBuffer[128];
	STextHelpers::Format(TitleBuffer, sizeof(TitleBuffer), "Debug Tool (Shift+X) %.3f ms/frame (%.1f FPS)###DebugTool", MsPerFrame, Framerate);
	
	ImGui::Begin(TitleBuffer);
	{
		DrawGUI();
	}
	ImGui::End();
	
	ImGui::Render();

	SDL_SetRenderLogicalPresentation(Renderer, 0, 0, SDL_LOGICAL_PRESENTATION_DISABLED);
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), Renderer);
}

void CDebugGUIModule::DrawGUI()
{
	ImGui::BeginTabBar("Main");
	for (IDebugTab* Tab : Tabs)
	{
		if (!!Tab && ImGui::BeginTabItem(Tab->GetTabName()))
		{
			Tab->OnDrawGUI();
			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();
}

IMPLEMENT_MODULE(CDebugGUIModule, DebugGUI)

