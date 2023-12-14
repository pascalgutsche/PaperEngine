#pragma once
#include "Engine.h"

struct ImFont;

namespace Paper
{

	struct FontConfig
	{
		std::string fontName;
		std::filesystem::path filePath;
		uint32_t fontSize = 18;
	};

	class ImGuiFont
	{
	public:
		static void Add(const FontConfig& config, bool isDefault = false);

		static void PushFont(const std::string& fontName);
		static void PopFont();

	private:
		static std::unordered_map<std::string, ImFont*> fonts;
	};
}

