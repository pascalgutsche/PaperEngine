#pragma once

#include <ImGui/imgui.h>

#define CONST_UI_ID ("##" + CutStringAfterLastSlash(__FILE__ + std::to_string(__LINE__))).c_str()


namespace Paper::UI
{
	const char* GenerateID();

	const char* GenerateLabelID(std::string_view label);


	struct ScopedStyle
	{
		ScopedStyle(const ScopedStyle&) = delete;
		ScopedStyle& operator=(const ScopedStyle&) = delete;
		template<typename T>
		ScopedStyle(ImGuiStyleVar styleVar, T value) { ImGui::PushStyleVar(styleVar, value); }
		~ScopedStyle() { ImGui::PopStyleVar(); }
	};

	struct ScopedColour
	{
		ScopedColour(const ScopedColour&) = delete;
		ScopedColour& operator=(const ScopedColour&) = delete;
		template<typename T>
		ScopedColour(ImGuiCol colourId, T colour) { ImGui::PushStyleColor(colourId, ImColor(colour).Value); }
		~ScopedColour() { ImGui::PopStyleColor(); }
	};

	struct ScopedFont
	{
		ScopedFont(const ScopedFont&) = delete;
		ScopedFont& operator=(const ScopedFont&) = delete;
		ScopedFont(ImFont* font) { ImGui::PushFont(font); }
		~ScopedFont() { ImGui::PopFont(); }
	};

	struct ScopedID
	{
		ScopedID(const ScopedID&) = delete;
		ScopedID& operator=(const ScopedID&) = delete;
		template<typename T>
		ScopedID(T id) { ImGui::PushID(id); }
		~ScopedID() { ImGui::PopID(); }
	};

	
}

std::string CutStringAfterLastSlash(std::string val);

