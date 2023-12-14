#include "Engine.h"
#include "ImGuiFont.h"

#include "ImGuiLayer.h"

namespace Paper
{
	std::unordered_map<std::string, ImFont*> ImGuiFont::fonts;

	void ImGuiFont::Add(const FontConfig& config, bool isDefault)
	{
		if (fonts.contains(config.fontName))
		{
			LOG_CORE_ERROR("Trying to add font: '{}' with path:'{}' but its name is already there.", config.fontName, config.filePath.string());
			return;
		}

		ImGuiIO& io = ImGui::GetIO();
		ImFont* font = io.Fonts->AddFontFromFileTTF(config.filePath.string().c_str(), config.fontSize, nullptr, io.Fonts->GetGlyphRangesDefault());

		if (!font)
		{
			LOG_CORE_ERROR("Failed to add font: '{}' with path:'{}'", config.fontName, config.filePath.string());
			return;
		}

		if (isDefault)
			io.FontDefault = font;
		
		fonts[config.fontName] = font;
	}

	void ImGuiFont::PushFont(const std::string& fontName)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (!fonts.contains(fontName))
		{
			LOG_CORE_ERROR("Trying to push font: '{}' but it's name wasn't found. ->  Pushing default Font", fontName);
			ImGui::PushFont(io.FontDefault);
			return;
		}

		ImFont* font = fonts.at(fontName);

		ImGui::PushFont(font);
	}

	void ImGuiFont::PopFont()
	{
		ImGui::PopFont();
	}
}
