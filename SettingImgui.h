#pragma once
#include "includes.h"


namespace SettingImgui
{
	void Initialize()
	{
		ImGui::SetItemDefaultFocus();
		ImGuiStyle* style = &ImGui::GetStyle();

		style->WindowRounding = 0.f;
		style->WindowTitleAlign = ImVec2(0.5f, 0.5f);

		style->Colors[ImGuiCol_WindowBg] = ImVec4(21.f / 255, 0.f / 255, 41.f / 255, 255.f / 255);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(135.f / 255, 0.f / 255, 255.f / 255, 255.f / 255);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(135.f / 255, 0.f / 255, 255.f / 255, 255.f / 255);
	}
}


