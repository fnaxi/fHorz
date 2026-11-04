// CopyRight FlareHorz Team. All Rights Reserved.


#include "Tabs/DebugTab.h"

#include "imgui.h"

void IDebugTab::BeginSection(const TCHAR* Name)
{
	ImGui::SeparatorText(Name);

	ImGui::PushID(Name);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 2.5f);

	const ImVec2 CurrentPadding = ImGui::GetStyle().WindowPadding;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(CurrentPadding.x + 5.0f, CurrentPadding.y + 5.0f));
	
	ImGui::BeginChild("Section", ImVec2(0.0f, 0.0f), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders, 0);
}

void IDebugTab::EndSection()
{	
	ImGui::EndChild();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopID();
	
	DrawEmptyLine();
}

void IDebugTab::DrawEmptyLine()
{
	ImGui::Text("");
}

