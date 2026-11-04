// CopyRight FlareHorz Team. All Rights Reserved.


#include "Tabs/ToolSettingsDebugTab.h"

#include "imgui.h"

void CToolSettingsDebugTab::OnDrawGUI()
{
	BeginSection("General ImGui style");
	{
		ImGui::SliderFloat("Transparency", &Transparency, 0.25f, 1.0f);
		
		ImGuiStyle& Style = ImGui::GetStyle();
		Style.Alpha = Transparency;
	}
	EndSection();

	//@TODO: make transparency work
}

